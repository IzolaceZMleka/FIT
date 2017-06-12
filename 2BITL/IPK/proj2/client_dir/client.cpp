// Pocitacove komunikace a site 2015/2016
// Projekt 2: Prenos souboru
// Jmeno a prijmeni: Jan Pavlica
// Login: xpavli78
// Datum: 24.4.2015
// Popis: client

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <sys/stat.h>

using namespace std;

typedef struct params // uchovani parametru
{
	bool download;
	bool upload;
	int port_number;
	const char* filename;
	const char* hostname;
} params;


int parse_arguments(int argc, const char* argv[], params *my_params);

int main( int argc, const char* argv[] )
{
	params my_params;
	int my_socket;
	hostent *host;
	sockaddr_in server_socket;
	
	if(parse_arguments(argc, argv, &my_params) != 0) // zracovani argumentu
	{
		return -1;
	}
	
	string pom = my_params.filename;
	
	if ((host = gethostbyname(my_params.hostname)) == NULL) // zjisteni serveru
	{
		cerr << "Spatna adresa." << endl;
		return -1;
	}
	
	if ((my_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) // vytvoreni socketu
	{
		cerr << "Nelze vytvorit soket" << endl;
		return -1;
	}
	

	server_socket.sin_family = AF_INET;
	server_socket.sin_port = htons(my_params.port_number);
	memcpy(&(server_socket.sin_addr), host->h_addr, host->h_length);
	
	my_params.filename = pom.c_str();
	
	if (connect(my_socket, (sockaddr *)&server_socket, sizeof(server_socket)) == -1) // pripojeni k serveru
	{
		cerr << "Nelze navazat spojeni." << endl;
		return -1;
	}
	
	
	if(my_params.upload) // UPLOAD
	{
		ifstream my_file;
		my_file.open (my_params.filename,ios::binary); // otevreni souboru
		
		if (my_file.is_open()) // pokud se povedlo
		{
			stringstream buffer;
			buffer << my_file.rdbuf();
			
			string text_in_file = buffer.str(); // ziskani dat
			my_file.close();
			
			string file = my_params.filename;
			
			std::string::size_type n;
			
			if((n = file.rfind("/")) != std::string::npos)
			{	
				file = file.substr(n+1);
			}
			
			size_t help = (text_in_file.size());
			string file_size;
			
			stringstream convert;
			
			convert << help;
			file_size = convert.str();
			
			string msg_u = "U|"+file_size+"|"+file; // retezec pro zaslani jmena a velikost souboru
			
			send(my_socket,msg_u.data(),msg_u.size(),0); // odeslani pozadavky o upload serveru
			
			char buf[3] = {0}; // buffer pro prijeti potvrzujici zpravy od serveru
			
			int size;
			
			while(1) // prijem kontrolni zpravy ze strany serveru
			{
				size = recv(my_socket,buf,sizeof(buf),0);
				string ok_msg = buf;
				ok_msg = ok_msg.substr(0,3);
				
				
				if(ok_msg == "OK1")
				{
					send(my_socket,text_in_file.data(),text_in_file.size(),0);
					break;
				}
				if(size == 0)
				{
					cerr << "Spojeni se serverem bylo ukonceno." << endl;
					close(my_socket);
					return -1;
				}
				else if(size == -1)
				{
					cerr << "Pri spojeni se vyskytl problem. Spojeni bylo ukonceno." << endl;
					close(my_socket);
					return -1;
				}
				
			}
			
			char buf2[3] = {0};
			
			while(1) // potvrteni, ze se soubor podarilo nahrat
			{
				size = recv(my_socket,buf2,sizeof(buf2),0);
				
				string ok_msg2 = buf2;
				ok_msg2 = ok_msg2.substr(0,3);
			
				if(ok_msg2 == "OK2")
				{
					break;
				}
				else if (ok_msg2 == "NOK")
				{
					cerr << "Soubor se nepovedlo uspesne nahrat. Client bude ukoncen." << endl;
					close(my_socket);
					return -1;
				}
				
				if(size == 0)
				{
					cerr << "Spojeni se serverem bylo ukonceno." << endl;
					close(my_socket);
					return -1;
				}
				else if(size == -1)
				{
					cerr << "Pri prenosu se vyskytl problem. Soubor nebyl uspesne nahran." << endl;
					close(my_socket);
					return -1;
				}
			}
			
		}
		else // v pripade neuspesneho otevreni souboru
		{
			cerr << "Pozadovany soubor se nepodarilo otevrit. Client bude ukoncen";
			close(my_socket);
			return -1;
		}
	}
	else if(my_params.download) // zadost o stazeni
	{
		string msg_u = "D||";
		msg_u = msg_u+my_params.filename;
		send(my_socket,msg_u.data(),msg_u.size(),0); // odeslani pozadavku pro download
		
		char ok_buf[1024] = {0};
		string text;
		
		recv(my_socket, ok_buf, 1024, 0);
		text = ok_buf;
		
		string control = text.substr(0,1);
		
		unsigned first = text.find('|');
		unsigned second = text.find('|',2);
		string info = text.substr ((first+1),second-(first+1));
		string d_f = text.substr (0,1);
		
		if(d_f == "D") // prijem zpravy od serveru o velikosti pozadovaneho souboru
		{
			stringstream convert(info); 
			int incoming_size;
			convert >> incoming_size;
			char msg_ok1[] = "OK1";
			
			send(my_socket,msg_ok1,sizeof(msg_ok1),0); // potvrzeni o prijeti velikosti souboru
			
			int recieved = 0;
			int size;
			
			text = "";
			
			string text_to_file;
			while (1) // prijem souboru
			{
// 				cout << "prijimam soubor" << endl;
				char buf[1024] = {0};
				
				if ((size = recv(my_socket, buf, 1024 - 1, 0)) == -1)
				{
					cerr << "Problem s prijetim dat" << endl;
					close(my_socket);
					return -1;
				}
				
				recieved += size;
				string pom;
				pom.assign(buf,size);
				text += pom;
				
				if(recieved == incoming_size)
				{
					break;
				}
				else if(size == 0)
				{
					cerr << "Prenos byl ukoncen ze strany clienta." << endl;
					close(my_socket);
					return -1;
				}
			}
			
			char msg_ok2[] = "OK2";
			send(my_socket,msg_ok2,sizeof(msg_ok2),0); // zprava potvrtujici prijeti souboru
			
			ofstream myfile;
			myfile.open(my_params.filename,ios::binary);
				
			if (myfile.is_open()) // zapis do souboru
			{
				myfile << text;
				myfile.close();
			}
			else
			{
				cerr << "Zapis do souboru se nezdaril. Client bude ukoncen." << endl;
				close(my_socket);
				return -1;
			}
		}
		else if(d_f == "F") // v pripade, ze soubor nebyl nalezen
		{
			cerr << "Pozdavoany soubor nebyl nalezen nebo k neni povolen pristup. Client bude ukoncen." << endl;
			close(my_socket);
			return -1;
		}
	}
	
	close(my_socket);
	return 0;
}

int parse_arguments(int argc, const char* argv[], params *my_params)
{

	if(argc != 7)
	{
		cerr << "Chybny pocet argumentu. Client bude ukoncen."<< endl <<"Postupujte podle vzoru:\nclient -h hostname -p port [-d|-u] file_name" << endl;
		return -1;
	}
	else
	{
		if(strcmp(argv[1],"-h") != 0)
		{
			cerr << "Chybi prepinac host (-h). Client bude ukoncen."<< endl <<"Postupujte podle vzoru:\nclient -h hostname -p port [-d|-u] file_name" << endl;
			return -1;
		}
		else
		{
			my_params->hostname = argv[2];
		}
		
		if(strcmp(argv[3],"-p") != 0)
		{
			cerr << "Chybi prepinac port (-p). Client bude ukoncen."<< endl <<"Postupujte podle vzoru:\nclient -h hostname -p port [-d|-u] file_name" << endl;
			return -1;
		}
		else
		{
			string port = argv[4];
			istringstream is(port);
			is >> my_params->port_number;
			if(my_params->port_number == 0)
			{
				cerr << "Chybne zadane cislo portu. Client bude ukoncen."<< endl <<"Postupujte podle vzoru:\nclient -h hostname -p port [-d|-u] file_name" << endl;
				return -1;
			}
		}
		
		if((strcmp(argv[5],"-d") == 0))
		{
			string control = argv[6];
			
			if(control.find("/") != string::npos)
			{
				cerr << "Nazev souboru obsahuje cestu k adresari. Client bude ukoncen.\nPostupujte podle vzoru:\nclient -h hostname -p port [-d|-u] file_name" << endl;
				return -1;
			}
			
			my_params->download = true;
			my_params->upload = false;
			my_params->filename = argv[6];
		}
		else if (strcmp(argv[5],"-u") == 0)
		{
			struct stat sb;
			
			lstat(argv[6],&sb);
			
			if(S_ISREG(sb.st_mode))
			{
			}
			else if(S_ISDIR(sb.st_mode))
			{
				return -1;
			}
			else if(S_ISLNK(sb.st_mode)) // zpracovani symbolickych odkazu
			{
				ssize_t r;

				char linkname[1024];
					if (linkname == NULL) {
						fprintf(stderr, "insufficient memory\n");
						exit(EXIT_FAILURE);
					}

				r = readlink(argv[6], linkname, 1024);

				if (r < 0) {
						cerr << "LSTAT ERROR" << endl;
						exit(EXIT_FAILURE);
					}

				linkname[r] = '\0';
				
				my_params->download = false;
				my_params->upload = true;
				my_params->filename = linkname;
				return 0;
			}
			
			my_params->download = false;
			my_params->upload = true;
			my_params->filename = argv[6];
		}
		else
		{
			cerr << "Chybi prepinac pro download neb upload [-d|-u]. Client bude ukoncen.\nPostupujte podle vzoru:\nclient -h hostname -p port [-d|-u] file_name" << endl;
			return -1;
		}
	}
	return 0;
}
