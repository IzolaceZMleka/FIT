// Pocitacove komunikace a site 2015/2016
// Projekt 2: Prenos souboru
// Jmeno a prijmeni: Jan Pavlica
// Login: xpavli78
// Datum: 24.4.2015
// Popis: server

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
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/wait.h>

using namespace std;

int parse_arguments(int argc, const char* argv[]);
int connect_and_listen(int port, int *sock, sockaddr_in *serv);

int main( int argc, const char* argv[] )
{
	int my_port = parse_arguments(argc, argv); // zpracovani portu
	
	if(my_port <= 0)
	{
		return -1;
	}
	
	int main_socket;
	struct sockaddr_in sock_name;
	
	connect_and_listen(my_port, &main_socket, &sock_name); // pripojeni
	
	int connected;
	pid_t pid;
	struct sockaddr_in cli_addr;
	int clilen = sizeof(cli_addr);
	
	while(1)
	{
		connected = accept(main_socket,(struct sockaddr *)&cli_addr, (socklen_t*) &clilen);
		
		if(connected != -1)
		{
			
			if( (pid = fork()) == 0) // vytvoreni noveho procesu
			{
				
				close(main_socket);
				int size;
				
				char ok_buf[1024] = {0};
				string text;
				
				recv(connected, ok_buf, 1024, 0); // prijem zadosti od clienta
				text = ok_buf;
				
				string control = text.substr(0,1);
				// zpracovani zadosti
				unsigned first = text.find('|');
				unsigned second = text.find('|',2);
				string info = text.substr ((first+1),second-(first+1));
				string filename = text.substr(second+1);

				if(control == "U") // upload
				{
					stringstream convert(info); 
					int incoming_size;
					convert >> incoming_size;
					char msg_ok1[] = "OK1";
					
					send(connected,msg_ok1,sizeof(msg_ok1),0); // potvrzeni prijeti velikosti
					
					int recieved = 0;
					
					text = "";
					
					bool is_ok = false;
					
					string text_to_file;
					while (1) // prijem souboru
					{
// 						cout << "prijimam soubor" << endl;
						char buf[1024] = {0};
						if ((size = recv(connected, buf, 1024 - 1, 0)) == -1)
						{
							cerr << "Problém s přijetím dat." << endl;
							break;
						}

						recieved += size;
						string pom;
						pom.assign(buf,size);
						text += pom;
						
						if(recieved == incoming_size)
						{
							is_ok = true;
							break;
						}
						else if(size == 0)
						{
							break;
						}
					}
					
					if(is_ok) // vytvoreni souboru
					{
						ofstream myfile;
						myfile.open(filename.c_str(),ios::binary);
						
						if (myfile.is_open())
						{
							myfile << text;
							myfile.close();
						}
						else
						{
							is_ok = false;
						}
					}
					
					if(!is_ok) // zprava potvrzujici uspech/neuspech nahrani souboru
					{
						char msg_ok2[] = "NOK";
						send(connected,msg_ok2,sizeof(msg_ok2),0);
					}
					else
					{
						char msg_ok2[] = "OK2";
						send(connected,msg_ok2,sizeof(msg_ok2),0);
					}
				}
				else if(control == "D") // download
				{
					ifstream my_file;
					my_file.open (filename.c_str(),ios::binary); // otevreni souboru
					
					if (my_file.is_open()) // pokud se povedlo
					{
						stringstream buffer;
						buffer << my_file.rdbuf();
						
						string text_in_file = buffer.str(); // ziskani dat
						
						size_t help = (text_in_file.size());
						string file_size;
						
						stringstream convert;
						
						convert << help;
						file_size = convert.str();
						
						
						string msg_u = "D|"+file_size+"|"; // retezec pro zaslani jmena a velikost souboru
						
						send(connected,msg_u.data(),msg_u.size(),0); // odeslani velikosti nalezeneho souboru
						
						char buf[3] = {0}; // buffer pro prijeti potvrzujici zpravy od clienta
						
						int size;
						
						while(1) // ocekavam potvrzeni od clienta, ze ma potrebne inforamce
						{
							size = recv(connected,buf,sizeof(buf),0);
							string ok_msg = buf;
							ok_msg = ok_msg.substr(0,3);
							
							
							if(ok_msg == "OK1")
							{
								send(connected,text_in_file.data(),text_in_file.size(),0);
								break;
							}
							if(size == 0)
							{
								cerr << "SPOJENI S KLIENTEM BYLO UKONCENO" << endl;
								break;
							}
							else if(size == -1)
							{
								cerr << "PRI SPOJENI SE VYSKYTL ERROR, SOUBOR NEBYL SPRAVNE ODESLAN" << endl;
								break;
							}
							
						}
					}
					else // v pripade, ze se nepodarilo soubor otevrit odesilame fail
					{
						string msg_u = "F|"; // retezec pro zaslani jmena a velikost souboru
						send(connected,msg_u.data(),msg_u.size(),0); // odeslani pozadavky o upload serveru
					}
					
					char buf2[3] = {0};
			
					while(1) // potvrzujici zprava od klienta pro ukonceni sojeni
					{
						size = recv(connected,buf2,sizeof(buf2),0);
						
						string ok_msg2 = buf2;
						ok_msg2 = ok_msg2.substr(0,3);
					
						if(ok_msg2 == "OK2")
						{
							break;
						}
						else if (ok_msg2 == "NOK")
						{
							cerr << "Soubor se nepovedlo uspesne nahrat. Client bude ukoncen." << endl;
							close(connected);
							exit(EXIT_FAILURE);
						}
						
						if(size == 0)
						{
							cerr << "Spojeni se serverem bylo ukonceno." << endl;
							close(connected);
							exit(EXIT_FAILURE);
						}
						else if(size == -1)
						{
							cerr << "Pri prenosu se vyskytl problem. Soubor nebyl uspense nahran." << endl;
							close(connected);
							exit(EXIT_FAILURE);
						}
					}
					
				}
				close(connected);
				exit(0);
			}
			
			close(connected);
		}
	}
}

int parse_arguments(int argc, const char* argv[]) // zpracovani argumentu
{
	if(argc != 3)
	{
		cerr << "Chybny pocet argumentu. Server bude ukoncen."<< endl <<"Postupujte podle vzoru:\nserver -p port" << endl;
		return -1;
	}
	else
	{
		if(strcmp(argv[1],"-p") != 0)
		{
			cerr << "Chybi prepinac port (-p). Server bude ukoncen."<< endl <<"Postupujte podle vzoru:\nserver -p port" << endl;
			return -1;
		}
		else
		{
			string port = argv[2];
			int port_number;
			istringstream is(port);
			is >> port_number;
			if(port_number != 0)
				return port_number;
			else
			{
				cerr << "Chybne zadane cislo portu. Server bude ukoncen."<< endl <<"Postupujte podle vzoru:\nserver -p port" << endl;
				return -1;
			}
		}
	}
	return 0;
}

int connect_and_listen(int port, int *sock, struct sockaddr_in *name) // vytvoreni socketu a fronty
{
	
	int main_socket;
	
	if((main_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		cerr << "Nelze vytvorti socket" << endl;
		return -1;
	}
	
	struct sockaddr_in sock_name;
	
	sock_name.sin_family = AF_INET;
	sock_name.sin_port = htons(port);
	sock_name.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(main_socket, (sockaddr *)&sock_name, sizeof(sock_name)) == -1)
	{
		cerr << "Problem s pojmenovanim socketu." << endl;
		return -1;
	}
	
	if (listen(main_socket, 10) == -1)
	{
		cerr << "Porblem s vytvorenim fronty." << endl;
		return -1;
	}
	
	*sock = main_socket;
	*name = sock_name;
	
	return 0;
}
