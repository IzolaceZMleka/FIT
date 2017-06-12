/* projekt IPK
 * xpavli78
 */

# include "webclient.h"

int main (int argc, char** argv)
{
	url to; // struktura pro uchovani informaci o URL
	
	if (argc != 2)
	{
			cerr << "ERROR SPATNE PARAMETRY" << endl;
			return 1;
	}
	else
	{
		string link = argv[1]; // promenna pro uchovani adresy
		if(get_url(link, to, FILE_NAME_T)) // parsovani URL
		{
			return 1;
		}
	}
	
// 	cout << "HOST: " + to.host << endl;
// 	cout << "CESTA: " + to.path << endl;
// 	cout << "PORT: " <<  to.port << endl;
// 	cout << "JMENO SOUBORU: "+to.file_name << endl;
	
	if(connect_to(to,HTTP_DEFAULT_1_1)) // pripojeni k zadane adrese
	{
		return 1;
	}
	return 0;
}

/* funkce pro parsovani URL */

int get_url(string link, url &to, bool file_name_get)
{
	size_t position; // promenna pro uchovani pozice nalezeneho podretezce
	to.full_link = link;
	
	if((position = link.find("http://")) != string::npos) // odstraneni http://
	{
		link.erase(0,7);
	}
	else
	{
		cerr << "Missing http" << endl;
		return 1;
	}
	if(file_name_get)
	{
		if((position = link.find_last_of("/")) != string::npos) // zjisteni jmena souboru, pred prekodovanim znaku
		{
			to.file_name = link.substr(position+1);
			if(to.file_name.empty()) // pokud je adresa konci lomitkem
			{
				to.file_name = "index.html";
			}
		}
		else
		{
			to.file_name = "index.html"; // pokud nebylo lomitko nalezeno vubec
		}
	}
	
	while((position = link.find(' ')) != string::npos) // prekodovani mezer
	{
		link.replace(position,1,"%20");
	}
	
	while((position = link.find('~')) != string::npos) // prekodovani ~
	{
		link.replace(position,1,"%7E");
	}
	
	if((position = link.find("/")) != string::npos) // cesta k souboru
	{
		to.path = link.substr(position);
		link.erase(position);
	}
	else
	{
		to.path = "/";
	}
	
	
	if((position = link.find(":")) != string::npos) // rozpoznani portu
	{
		stringstream s(link.substr(position+1));
		s >> to.port;
		link.erase(position);
	}
	
	to.host = link;
	return 0;
}

int connect_to(url &to, bool http_1_1)
{
	string get_request{ // zadost pro http 1.1
		"GET " + to.path + " HTTP/1.1\r\n" +
		"Host: " + to.host + "\r\n" +
		"Connection: close \r\n\r\n"
		};
		
	string get_request_1_0{ // zadost pro http 1.0
		"GET " + to.path + " HTTP/1.0\r\n" +
		"Host: " + to.host + "\r\n" +
		"Connection: close \r\n\r\n"
		};
		
	hostent *host;
	sockaddr_in server_socket;
	int my_socket;
	int size;
	
	if ((my_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) // nastaveni socketu
	{
		cerr << "Socket failed" << endl;
		return 1;
	}
	
	if ((host = gethostbyname(to.host.c_str())) == NULL) // zjisteni hosta
	{
		cerr << "Host failed" << endl;
		return 1;
	}
	
	server_socket.sin_family = AF_INET;
	server_socket.sin_port = htons(to.port);
	memcpy(&(server_socket.sin_addr), host->h_addr, host->h_length);
	
	if (connect(my_socket, (sockaddr*)&server_socket, sizeof(server_socket)) == -1) // pripojeni
	{
		cerr << "Connect failed" << endl;
		return 1;
	}
	
	
	if(http_1_1) // http_1_1 Request
	{
		if ((size = send(my_socket, get_request.c_str(), get_request.size(), 0)) == -1)
		{
			cerr << "Send failed" << endl;
			return 1;
		}	
	}
	else // http_1_0 Request
	{
		if ((size = send(my_socket, get_request_1_0.c_str(), get_request_1_0.size(), 0)) == -1)
		{
			cerr << "Send failed" << endl;
			return 1;
		}
	}
	
	if(get_content(my_socket, to)) // po pripojeni nacteni obsahu
	{
		return 1;
	}
	
	close(my_socket); 
	return 0;
}

int get_content(int my_socket, url &to)
{
	char buffer[BUFSIZE];
	size_t position; // promenna pro ulozeni pozice nalezeneho podretezce
	string text; // promenna pro uchovani textu
	int i;
	
	string header; // promenna pro uchovani hlavicky
	bool header_found = false; // nalezena hlavicka
	
	string code; // promenna pro uchovani ziskaneho kodu (200, 301, 302, 404, etc)
	
	bool http_1_1 = true;
	string http_1_0 = "0";
	
	static int redir_counter; // pocitadlo redirectu
	
	while((i = recv(my_socket, &buffer, BUFSIZE, 0)) > 0) // nacitani prijatych dat
	{
		text.append(buffer,i);
		
		if((position = text.find("\r\n\r\n")) != string::npos && !header_found) // nalezeni hlavicky
		{
			header = text.substr(0,position); // ziskani hlavicky
			text.erase(0,position+4); // uriznuti hlavicky
			header_found = true;
		}
	}
	
//cout << header << endl << endl;
	
	if(header.substr(8,1) == http_1_0) // kontrola zda neni http 1.0
	{
		http_1_1 = false;
		close(my_socket);
		connect_to(to,http_1_1); // pripadny request http 1.0
		return 0;
	}
	
	int code_i = check_code(code = header.substr(9,3)); // ziskani kodu a jeho kontrola
	
	if(code_i == 1) // redirect 301
	{
		redir_counter++;
		
		if((position = header.find("\r\nLocation: ")) != string::npos && redir_counter != 6)
		{
			close(my_socket);
			string new_link = header.substr(position+12,header.substr(position+12).find("\r\n"));
			get_url(new_link,to,FILE_NAME_F);
			connect_to(to,HTTP_DEFAULT_1_1);
			return 0;
		}
		else
		{
			if(redir_counter == 6)
			{
				cerr << "More than 5 redirects" << endl;
				return 1;
			}
			cerr << "Missing LOCATION" << endl;
			return 1;
		}
	}
	else if(code_i == 2) // redirect 302
	{
		redir_counter++;
		
		static map<string,string> redirected_urls; // ulozeni predchozich presmerovani
		
		int redirs_302 = 0;
		
		if((position = header.find("\r\nLocation: ")) != string::npos && redir_counter != 6)
		{
			close(my_socket);
			
			string new_link = header.substr(position+12,header.substr(position+12).find("\r\n"));
			
			redirected_urls[to.full_link] = new_link;
			
			while(redirected_urls.find(new_link) != redirected_urls.end())
			{
				new_link = redirected_urls[new_link];
				
				redirs_302++;
				
				if(redirs_302 > 6)
				{
					cerr << "302 cycled" << endl;
					return 1;
				}
			}
			
			get_url(new_link,to,FILE_NAME_F);
			connect_to(to,HTTP_DEFAULT_1_1);
			return 0;
		}
		else
		{
			if(redir_counter == 6)
			{
				cerr << "More than 5 redirects" << endl;
				return 1;
			}
			cerr << "Missing LOCATION" << endl;
			return 1;
		}
	}
	else if(code_i == 3) // pri erroru
	{
		close(my_socket);
		return 1;
	}
	else if(code_i == 4) // pri 400 nebo 505
	{
		http_1_1 = false;
		close(my_socket);
		connect_to(to,http_1_1); // pripadny request http 1.0
		return 0;
	}
	
	if((position = header.find("Transfer-Encoding: chunked")) != string::npos) // kdyz jsu data chunked
	{
		text = dechunk(text);
	}
	
	/* nahrani dat do souboru */
	
	ofstream myfile;
	myfile.open (to.file_name);
	myfile << text;
	myfile.close();
	
	return 0;
}

/* dekodovani chunked dat */

string dechunk(string text)
{
	size_t w_split = text.find("\r\n"); // nalezeni prvniho chunku
	
	/* zjisteni jeho velikosti */
	
	int ch_size;
	stringstream s;
	s << hex << text.substr(0,w_split);
	s >> ch_size;
	string d_text = text.substr(w_split+2,ch_size);
	
	/* opakovany postup, dokud nejdeme na konec */
	
	while(ch_size != 0)
	{
		text.erase(0,w_split+2);
		text.erase(0,2+ch_size);
		
		w_split = text.find("\r\n");
		stringstream s;
		s << hex << text.substr(0,w_split);
		s >> ch_size;
		
		d_text += text.substr(w_split+2,ch_size);
	}
	
	return d_text;
	
}

/* rozpoznani kodu */

int check_code(string code){
	
	stringstream s(code);
	int code_i;
	s >> code_i;
	
	switch(code_i)
	{
		case 200: return 0;
		case 301: return 1;
		case 302: return 2;
		case 400: return 4;
		case 401: cerr << "Unauthorized" << endl; return 3;
		case 402: cerr << "Payment Required" << endl; return 3;
		case 403: cerr << "Forbidden" << endl; return 3;
		case 404: cerr << "Not Found" << endl; return 3;
		case 405: cerr << "Method Not Allowed" << endl; return 3;
		case 406: cerr << "Not Acceptable" << endl; return 3;
		case 407: cerr << "Proxy Authentication Required" << endl; return 3;
		case 408: cerr << "Request Timeout" << endl; return 3;
		case 409: cerr << "Conflict" << endl; return 3;
		case 410: cerr << "Gone" << endl; return 3;
		case 411: cerr << "Length Required" << endl; return 3;
		case 412: cerr << "Precondition Failed" << endl; return 3;
		case 413: cerr << "Payload Too Large" << endl; return 3;
		case 414: cerr << "URI Too Long" << endl; return 3;
		case 415: cerr << "Unsupported Media Type" << endl; return 3;
		case 416: cerr << "Range Not Satisfiable" << endl; return 3;
		case 417: cerr << "Expectation Failed" << endl; return 3;
		case 418: cerr << "I'm a teapot" << endl; return 3;
		case 421: cerr << "Misdirected Request" << endl; return 3;
		case 422: cerr << "Unprocessable Entity" << endl; return 3;
		case 423: cerr << "Locked" << endl; return 3;
		case 424: cerr << "Failed Dependency" << endl; return 3;
		case 426: cerr << "Upgrade Required" << endl; return 3;
		case 428: cerr << "Precondition Required" << endl; return 3;
		case 429: cerr << "Too Many Requests" << endl; return 3;
		case 431: cerr << "Request Header Fields Too Large" << endl; return 3;
		case 451: cerr << "Unavailable For Legal Reasons" << endl; return 3;

		case 500: cerr << "Internal Server Error" << endl; return 3;
		case 501: cerr << "Not Implemented" << endl; return 3;
		case 502: cerr << "Bad Gateway" << endl; return 3;
		case 503: cerr << "Service Unavailable" << endl; return 3;
		case 504: cerr << "Gateway Timeout" << endl; return 3;
		case 505: return 4;
		case 506: cerr << "Variant Also Negotiates" << endl; return 3;
		case 507: cerr << "Insufficient Storage" << endl; return 3;
		case 508: cerr << "Loop Detected" << endl; return 3;
		case 510: cerr << "Not Extended" << endl; return 3;
		case 511: cerr << "Network Authentication Required" << endl; return 3;
		default: return 5;
	}
}
