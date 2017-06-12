/*Author: Jan Pavlica, xpavli78, VUT FIT
Description: This packet sniffer can analyze CDP and LLDP packets.
             It can also sends CDP and LLDP packets on specified interface.*/

#include "isa2.h"

tlvs my; // promenna pro ulozeni 

void print_packet(const struct pcap_pkthdr* pkthdr,const u_char* packet); // tisk packetu
void send_lldp(char *dev); // odeslání lldp packetu
void send_cdp(char *dev); // odeslan cdp packetu
uint16_t lldp_tlv_t_l(unsigned int tlv_type, unsigned int tlv_length); // funkce pro utvoreni 16 bitu pro pole length a type u LLDP TVL polozek

int main(int argc, char *argv[])
{
	int c;
	my.hello = false;
	// parsovani argumetnu
	while (1)
	{
		static struct option long_options[] =
			{
			{"send-hello",     no_argument,       NULL, 's'},
			{"ttl",  required_argument,       NULL, 't'},
			{"duplex",  required_argument, NULL, 'd'},
			{"software-version",  required_argument, NULL, 'o'},
			{"device-id",    required_argument, NULL, 'e'},
			{"platform",    required_argument, NULL, 'p'},
			{"port-id",    required_argument, NULL, 'q'},
			{"capabilities",    required_argument, NULL, 'c'},
			{"address",    required_argument, NULL, 'a'},
			{0, 0, 0, 0}
			};
			
		int option_index = 0;

		c = getopt_long (argc, argv, "hi:st:d:o:e:p:q:c:a:?", long_options, &option_index);

		if (c == -1)
			break;

		switch (c)
			{
			case 0:
				break;
			case '?':
			case 'h':
				cout << "----------------------------------------------" << endl;
				cout << "                     Help                     " << endl;
				cout << "----------------------------------------------" << endl;
				cout << "Author: Jan Pavlica, xpavli78, VUT FIT" << endl;
				cout << "Description: These packet sniffer can analyze CDP and LLDP packets.\n            It can also sends CDP and LLDP packets on specified interface." << endl;
				cout << "Usage: ./myL2monitor -i <interface> {--send-hello {--ttl <seconds>} {--duplex [full|half]} {--software-version <version>} {--device-id <identifier>} {--platform <platform>} {--port-id <interface>} {--capabilities <integer>} {--address <IPv4>}}" << endl;
				cout << "\t -i <interface> - interface you want sniff on" << endl;
				cout << "\t --send-hello - type for sending packets" << endl;
				cout << "\t --ttl <seconds> - Time To Live (default 180s)" << endl;
				cout << "\t --duplex [full|half] - duplex specification" << endl;
				cout << "\t --software-version <version> - version of software" << endl;
				cout << "\t --device-id <identifier> - device ID" << endl;
				cout << "\t --platform <platform> - platform" << endl;
				cout << "\t --port-id <interface> - port ID" << endl;
				cout << "\t --capabilities <integer> - device capabilities" << endl;
				cout << "\t --address <IPv4> - IPv4 address of device" << endl;
				return 0;
			case 's': // zapnuto odesilani packetu
				struct utsname unameData;
				uname(&unameData);
				
				my.hello = true; // je zapnuto odesilani
				// nastaveni vychozich hodnot
				my.ttl = TTL_DEFAULT;
				my.duplex = DUPLEX_FULL;
				my.software_version.append(unameData.sysname);
				my.software_version.append(" ");
				my.software_version.append(unameData.nodename);
				my.software_version.append(" ");
				my.software_version.append(unameData.release);
				my.software_version.append(" ");
				my.software_version.append(unameData.version);
				my.software_version.append(" ");
				my.software_version.append(unameData.machine);
				
				/* printf("%s %s %s %s %s", unameData.sysname,unameData.nodename,unameData.release,unameData.version,unameData.machine);*/
				char hostname[HOSTNAME_SIZE];
				gethostname(hostname, sizeof hostname);
				my.device_id = hostname;
				my.platform = unameData.sysname;
				my.port_id = my.dev;
				my.capabilities_cdp = CDP_CAPABILITIES;
				my.capabilities_lldp = LLDP_CAPABILITIES;
				
				int fd;
				struct ifreq ifr;

				fd = socket(AF_INET, SOCK_DGRAM, 0);

				/* zjisteni IPv4 adresy */
				ifr.ifr_addr.sa_family = AF_INET;
				strncpy(ifr.ifr_name, my.dev, IFNAMSIZ-1);
				ioctl(fd, SIOCGIFADDR, &ifr);
				close(fd);
				my.ipv4 = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
				break;
			
			case 'i': // device parametr
				my.dev = optarg;
				break;

			case 't': // ttl parametr
				if(my.hello)
				{
					my.ttl = atoi(optarg);
				}
				else
				{
					cout << "Sender mod is not enabled! If you wanna send packet type \"--send-hello\"" << endl;
					cout << "Program will exit." << endl;
					return 1;
				}	
				break;

			case 'd': // duplex parametr
				if(my.hello)
				{
					if (strcmp(optarg, "half") == 0) 
					{ // match!
						my.duplex = 0;
					}
					else if (strcmp(optarg, "full") == 0) 
					{ // not matched 
						my.duplex = 1;
					}
					else
					{
						cout << "Argument for duplex must be \'half\' or \'full\'."  << endl;
						cout << "Program will exit." << endl;
						return 1;
					}
				}
				else
				{
					cout << "Sender mod is not enabled! If you wanna send packet type \"--send-hello\"" << endl;
					cout << "Program will exit." << endl;
					return 1;
				}	
				break;

			case 'o': // software version parametr
				if(my.hello)
				{
					my.software_version = optarg;
				}
				else
				{
					cout << "Sender mod is not enabled! If you wanna send packet type \"--send-hello\"" << endl;
					cout << "Program will exit." << endl;
					return 1;
				}	
				break;

			case 'e': // device id parametr
				if(my.hello)
				{
					my.device_id = optarg;
				}
				else
				{
					cout << "Sender mod is not enabled! If you wanna send packet type \"--send-hello\"" << endl;
					cout << "Program will exit." << endl;
					return 1;
				}	
				break;
			
			case 'p': // platform parametr
				if(my.hello)
				{
					my.platform = optarg;
				}
				else
				{
					cout << "Sender mod is not enabled! If you wanna send packet type \"--send-hello\"" << endl;
					cout << "Program will exit." << endl;
					return 1;
				}	
				break;
			
			case 'q': // port id parametr
				if(my.hello)
				{
					my.port_id = optarg;
				}
				else
				{
					cout << "Sender mod is not enabled! If you wanna send packet type \"--send-hello\"" << endl;
					cout << "Program will exit." << endl;
					return 1;
				}	
				break;
			
			case 'c': // capabilities parametr
				if(my.hello)
				{
					my.capabilities_cdp = atoi(optarg);
					my.capabilities_lldp = atoi(optarg);
				}
				else
				{
					cout << "Sender mod is not enabled! If you wanna send packet type \"--send-hello\"" << endl;
					cout << "Program will exit." << endl;
					return 1;
				}	
				break;
			
			case 'a': // address parametr
				if(my.hello)
				{
					my.ipv4 = optarg;
				}
				else
				{
					cout << "Sender mod is not enabled! If you wanna send packet type \"--send-hello\"" << endl;
					cout << "Program will exit." << endl;
					return 1;
				}	
				break;
			
			default:
				printf("Wrong parametr. Program will exit.\n");
				return 1;
			}
	}
	/*
	cout << "DEVICE:\t\t" << my.dev << endl;
	cout << "HELLO:\t\t" << my.hello << endl;
	cout << "TTL:\t\t" << my.ttl << endl;
	cout << "DUPLEX:\t\t" << my.duplex << endl;
	cout << "SV:\t\t" << my.software_version << endl;
	cout << "device_id:\t" << my.device_id << endl;
	cout << "platform:\t" << my.platform << endl;
	cout << "port_id:\t" << my.port_id << endl;
	cout << "capabilities:\t" << my.capabilities_lldp << "\t" << my.capabilities_cdp << endl;
	cout << "ipv4:\t\t" << my.ipv4 << endl;
*/
	
	char errbuf[PCAP_ERRBUF_SIZE]; // buffer pro errory
	pcap_t *handle;
	struct bpf_program fp;
	bpf_u_int32 netp = 0;
	cout << my.dev;
	/* otevreni zarizeni pro sniffing */
	handle = pcap_open_live(my.dev, BUFSIZ, 1, 1000, errbuf);
	if (handle == NULL) {
		fprintf(stderr, "Couldn't open device %s: %s\n", my.dev, errbuf);
		return(2);
	}
	// filter pro odchytavani CDP a LLDP packetu
	pcap_compile(handle,&fp,"ether proto 0x88cc or (ether[12:2] <= 1500 && ether[14:2] == 0xAAAA && ether[16:1] == 0x03 && ether[17:2] == 0x0000 && ether[19:1] == 0x0C && ether[20:2] == 0x2000)",0,netp);
	pcap_setfilter(handle,&fp);
	
	// pomocne promenne pro odchytavani packetu	
	int res;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	
	//pomocne promenne pro zjistovani casu
	time_t s_time, c_time;
	time( &s_time );
	bool cdp_s = false;
	
	//odelsani prvnich packetu
	if(my.hello)
	{
		send_lldp(my.dev);
		send_cdp(my.dev);
	}
	
	//smycka pro odchytavani packetu
	while((res = pcap_next_ex( handle, &header, &pkt_data)) >= 0)
	{
		if(my.hello) // odesilani packetu
		{
			// zjisteni aktualniho casu
			time(&c_time);
			if(c_time - s_time >= 30) // co 30 vterin odesli lldp
			{
				send_lldp(my.dev);
				
				
				if(cdp_s) // co 60 vterin odesli cdp
				{
					cdp_s = false;
					send_cdp(my.dev);
				}
				else
				{
					cdp_s = true;
				}
				
				time( &s_time ); // nastav cas pro kontrolu
			}
		}
		
		if(res == 0)
		{
			continue;
		}
		else if(res == 1)
		{
			// tisk packetu 
			print_packet(header,pkt_data);
		}
		else if(res == -1)
		{
			pcap_geterr(handle);
		}
		 
		
	}
	
	//pcap_loop(handle,-1,print_packet,NULL);
	
}

uint16_t lldp_tlv_t_l(unsigned int tlv_type, unsigned int tlv_length)
{
	bitset<7> type = tlv_type;
	bitset<9> length = tlv_length;
	bitset<16> cust = concat(type,length);
	return (uint16_t)cust.to_ulong();
}

// kostra prevzata z https://gist.github.com/austinmarton/1922600

void send_lldp(char *dev)
{
	// pomocne promenne
	int sockfd;
	struct ifreq if_idx;
	struct ifreq if_mac;
	int tx_len = 0;
	unsigned char sendbuf[BUF_SIZ];
	struct ether_header *eh = (struct ether_header *) sendbuf;
	struct sockaddr_ll socket_address;
	char ifName[IFNAMSIZ];
	
	// ziskani interface
	strcpy(ifName, dev);

	// otevreni RAW socketu
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
	    perror("socket");
	}

	// zjisteni indexu zarizeni
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
	    perror("SIOCGIFINDEX");
	
	// zjisteni mac adresy zarizeni
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
	    perror("SIOCGIFHWADDR");

	// vynulovani bufferu
	memset(sendbuf, 0, BUF_SIZ);
	
	// ethernet hlavicka
	eh->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
	eh->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
	eh->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
	eh->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
	eh->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
	eh->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
	eh->ether_dhost[0] = MY_DEST_MAC0;
	eh->ether_dhost[1] = MY_DEST_MAC1;
	eh->ether_dhost[2] = MY_DEST_MAC2;
	eh->ether_dhost[3] = MY_DEST_MAC3;
	eh->ether_dhost[4] = MY_DEST_MAC4;
	eh->ether_dhost[5] = MY_DEST_MAC5;
	
	// ethertype pole
	eh->ether_type = htons(0x88cc); // protokol pro lldp
	tx_len += sizeof(struct ether_header);

	/* Packet data */
	/*Chassis ID */
	sendbuf[tx_len++] = 0x02;
	sendbuf[tx_len++] = 0x07;
	sendbuf[tx_len++] = 0x04;
	/* MAC */
	sendbuf[tx_len++] = eh->ether_shost[0];
	sendbuf[tx_len++] = eh->ether_shost[1];
	sendbuf[tx_len++] = eh->ether_shost[2];
	sendbuf[tx_len++] = eh->ether_shost[3];
	sendbuf[tx_len++] = eh->ether_shost[4];
	sendbuf[tx_len++] = eh->ether_shost[5];
	
	/*Port ID */
	
	uint16_t type_lenght = lldp_tlv_t_l(LLDP_TYPE_PORT_ID,sizeof(dev)-1); // promenna do ktere ukladam ziskane 2 byte pro type a length
	
	sendbuf[tx_len++] = (char)(type_lenght >> 8);
	sendbuf[tx_len++] = (char)(type_lenght);
	sendbuf[tx_len++] = 0x05;
	
	for(unsigned int i = 0; i < sizeof(dev)-1; i++)
	{
		sendbuf[tx_len+i] = dev[i];
	}
	
	tx_len+=sizeof(dev)-2;
	
	/* TTL */
	
	type_lenght = lldp_tlv_t_l(LLDP_TYPE_TTL,LLDP_SIZE_TTL);
	
	sendbuf[tx_len++] = (char)(type_lenght >> 8);
	sendbuf[tx_len++] = (char)(type_lenght);
	
	sendbuf[tx_len++] = (char)(my.ttl >> 8);
	sendbuf[tx_len++] = (char)(my.ttl);
	
	/* System Name */
	
	type_lenght = lldp_tlv_t_l(LLDP_TYPE_SYSTEM_NAME,strlen(my.device_id));
	
	sendbuf[tx_len++] = (char)(type_lenght >> 8);
	sendbuf[tx_len++] = (char)(type_lenght);
	
	for(unsigned int i = 0; i < strlen(my.device_id); i++)
	{
		sendbuf[tx_len+i] = my.device_id[i];
	}
	
	tx_len+=strlen(my.device_id);
	
	/* System Description */
	const char *sys_desc = my.software_version.c_str();
	
	type_lenght = lldp_tlv_t_l(LLDP_TYPE_SYSTEM_DESCRIPTION,strlen(sys_desc));
	
	sendbuf[tx_len++] = (char)(type_lenght >> 8);
	sendbuf[tx_len++] = (char)(type_lenght);
	
	for(unsigned int i = 0; i < strlen(sys_desc); i++)
	{
		sendbuf[tx_len+i] = sys_desc[i];
	}
	
	tx_len+=strlen(sys_desc);
	
	
	/*System Capabilities*/
	
	type_lenght = lldp_tlv_t_l(LLDP_TYPE_CAPABILITIES,LLDP_SIZE_CAPABILITIES);
	
	sendbuf[tx_len++] = (char)(type_lenght >> 8);
	sendbuf[tx_len++] = (char)(type_lenght);
	
	sendbuf[tx_len++] = (char)(my.capabilities_lldp >> 8);
	sendbuf[tx_len++] = (char)(my.capabilities_lldp);
	
	sendbuf[tx_len++] = (char)(my.capabilities_lldp >> 8);
	sendbuf[tx_len++] = (char)(my.capabilities_lldp);
	/*Management Address*/
	
	type_lenght = lldp_tlv_t_l(LLDP_TYPE_MANAGEMENT_ADDRESS,LLDP_SIZE_MANAGEMENT_ADDRESS);
	
	sendbuf[tx_len++] = (char)(type_lenght >> 8);
	sendbuf[tx_len++] = (char)(type_lenght);
	sendbuf[tx_len++] = 0x05;
	sendbuf[tx_len++] = 0x01;
	
	char value = 0;
    char *str = my.ipv4; /* save the pointer */
    while (*str) {
        if (isdigit((unsigned char)*str)) {
            value *= 10;
            value += *str - '0';
        } else {
            sendbuf[tx_len++] = (char)value;
			value = 0;
        }
        str++;
    }
    sendbuf[tx_len++] = (char)value;
	sendbuf[tx_len++] = 0x02;
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = 0x02;
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = 0x00;

	/* Index of the network device */
	socket_address.sll_ifindex = if_idx.ifr_ifindex;
	/* Address length*/
	socket_address.sll_halen = ETH_ALEN;
	/* Destination MAC */
	socket_address.sll_addr[0] = MY_DEST_MAC0;
	socket_address.sll_addr[1] = MY_DEST_MAC1;
	socket_address.sll_addr[2] = MY_DEST_MAC2;
	socket_address.sll_addr[3] = MY_DEST_MAC3;
	socket_address.sll_addr[4] = MY_DEST_MAC4;
	socket_address.sll_addr[5] = MY_DEST_MAC5;

	/* Send packet */
	if (sendto(sockfd, sendbuf, tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0)
	    printf("Send failed\n");
	
	return;
}

// kostra prevzata z https://gist.github.com/austinmarton/1922600
void send_cdp(char *dev)
{
	// pomocne promenne
	int sockfd;
	struct ifreq if_idx;
	struct ifreq if_mac;
	int tx_len = 0;
	unsigned char sendbuf[BUF_SIZ];
	struct ether_header *eh = (struct ether_header *) sendbuf;
	struct sockaddr_ll socket_address;
	char ifName[IFNAMSIZ];
	
	// ziskani interface
	strcpy(ifName, dev);

	// otevreni RAW socketu
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
	    perror("socket");
	}

	// zjisteni indexu zarizeni
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
	    perror("SIOCGIFINDEX");
	
	// zjisteni mac adresy zarizeni
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
	    perror("SIOCGIFHWADDR");

	// vynulovani bufferu
	memset(sendbuf, 0, BUF_SIZ);
	
	// ethernet hlavicka
	eh->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
	eh->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
	eh->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
	eh->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
	eh->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
	eh->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
	eh->ether_dhost[0] = MY_DEST_MAC0_CDP;
	eh->ether_dhost[1] = MY_DEST_MAC1_CDP;
	eh->ether_dhost[2] = MY_DEST_MAC2_CDP;
	eh->ether_dhost[3] = MY_DEST_MAC3_CDP;
	eh->ether_dhost[4] = MY_DEST_MAC4_CDP;
	eh->ether_dhost[5] = MY_DEST_MAC5_CDP;
	
	tx_len += sizeof(struct ether_header);
	
	sendbuf[tx_len++] = 0xaa;
	sendbuf[tx_len++] = 0xaa;
	sendbuf[tx_len++] = 0x03;
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = 0x0c;
	sendbuf[tx_len++] = 0x20;
	sendbuf[tx_len++] = 0x00;

	/* Packet data */
	/* TTL */
	sendbuf[tx_len++] = 0x02;
	sendbuf[tx_len++] = (char)(my.ttl);
	
	/* checksum pred vypoctem*/
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = 0x00;
	
	/*Device name*/
	
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = CDP_TYPE_DEVICE_NAME;
	
	sendbuf[tx_len++] = (char)(((strlen(my.device_id))+4) >> 8);
	sendbuf[tx_len++] = (char)((strlen(my.device_id))+4);
	
	for(unsigned int i = 0; i < strlen(my.device_id); i++)
	{
		sendbuf[tx_len+i] = my.device_id[i];
	}
	
	tx_len+=strlen(my.device_id);
	
	/*Software version*/
	
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = CDP_TYPE_DEVICE_VERSION;
	
	sendbuf[tx_len++] = (char)(((strlen(my.software_version.c_str()))+4) >> 8);
	sendbuf[tx_len++] = (char)((strlen(my.software_version.c_str()))+4);
	
	for(unsigned int i = 0; i < strlen(my.software_version.c_str()); i++)
	{
		sendbuf[tx_len+i] = (my.software_version.c_str())[i];
	}
	
	tx_len+=strlen(my.software_version.c_str());
	
	/*Platform*/
	
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = CDP_TYPE_DEVICE_PLATFORM;
	
	sendbuf[tx_len++] = (char)(((strlen(my.platform))+4) >> 8);
	sendbuf[tx_len++] = (char)((strlen(my.platform))+4);
	
	for(unsigned int i = 0; i < strlen(my.platform); i++)
	{
		sendbuf[tx_len+i] = my.platform[i];
	}
	
	tx_len+=strlen(my.platform);
	
	/*Port ID*/
	
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = CDP_TYPE_DEVICE_PORT_ID;
	
	sendbuf[tx_len++] = (char)(((strlen(my.port_id))+4) >> 8);
	sendbuf[tx_len++] = (char)((strlen(my.port_id))+4);
	
	for(unsigned int i = 0; i < strlen(my.port_id); i++)
	{
		sendbuf[tx_len+i] = my.port_id[i];
	}
	
	tx_len+=strlen(my.port_id);
	
	/*Duplex*/
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = CDP_TYPE_DEVICE_DUPLEX;
	
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = 0x05;
	
	sendbuf[tx_len++] = (char)my.duplex;
	
	/*System Capabilities*/
	
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = CDP_TYPE_DEVICE_CAPABILITIES;
	
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = 0x08;
	
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = (char)(my.capabilities_cdp);
	
	/*Addresses */
	
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = CDP_TYPE_DEVICE_ADDRESS;
	
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = 0x11;
	
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = 0x01;
	
	sendbuf[tx_len++] = 0x01;
	
	sendbuf[tx_len++] = 0x01;
	sendbuf[tx_len++] = 0xcc;
	
	sendbuf[tx_len++] = 0x00;
	sendbuf[tx_len++] = 0x04;	
	
	char value = 0;
    char *str = my.ipv4; /* ulozeni pointeru */
    
    while (*str) {
        if (isdigit((unsigned char)*str)) {
            value *= 10;
            value += *str - '0';
        } else {
            sendbuf[tx_len++] = (char)value;
			value = 0;
        }
        str++;
    }
    sendbuf[tx_len++] = (char)value;
	
	/* vypocet checksum */
	
	unsigned int checksum = 0;
	int i;
	
	if(tx_len % 2 == 0) // pri sudem poctu bytu
	{
		for(i = CDP_BEFORE; i < tx_len; i+=2) 
		{
			checksum += ((((sendbuf[i] & 0xff) << 8) + (sendbuf[i+1] & 0xff)) & 0xffff);
		}
	}
	else // pri lichem poctu bytu
	{
		for(i = CDP_BEFORE; i < (tx_len-1); i+=2)
		{
			checksum += ((((sendbuf[i] & 0xff) << 8) + (sendbuf[i+1] & 0xff)) & 0xffff);
		}
		char padding[2]; // posledni byte 
		padding [0] = 0x00;
		padding [1] = (sendbuf[tx_len-1] & 0xff);
		checksum += ((((padding[0] & 0xff) << 8) + (padding[1] & 0xff)) & 0xffff);
	}

	while(checksum > 0xffff) // pokud doslo k preteceni
	{
		checksum = (checksum >> 16) + (checksum & 0xffff);
	}

	uint16_t checksum_final = (uint16_t)(~checksum); // negace
	
	sendbuf[24] = (char)(checksum_final >> 8);
	sendbuf[25] = (char)checksum_final;
	
	// nastaveni velikosti CDP packetu
	eh->ether_type = htons(tx_len-14);
	
	
	
	/* Index of the network device */
	socket_address.sll_ifindex = if_idx.ifr_ifindex;
	/* Address length*/
	socket_address.sll_halen = ETH_ALEN;
	/* Destination MAC */
	socket_address.sll_addr[0] = MY_DEST_MAC0;
	socket_address.sll_addr[1] = MY_DEST_MAC1;
	socket_address.sll_addr[2] = MY_DEST_MAC2;
	socket_address.sll_addr[3] = MY_DEST_MAC3;
	socket_address.sll_addr[4] = MY_DEST_MAC4;
	socket_address.sll_addr[5] = MY_DEST_MAC5;

	/* Send packet */
	if (sendto(sockfd, sendbuf, tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0)
	    printf("Send failed\n");
	
	return;
}

void print_packet(const struct pcap_pkthdr* pkthdr,const u_char* packet)
{
	
	bool type, length;
	unsigned int type_n, length_n;
	
	type = true;
	length = true;
	
	const u_char* data = packet;
	int size = pkthdr->len;
	
	unsigned int prot = (data[12] << 8) + data[13];
	
	if(prot == 0x88cc) // tisk pro LLDP
	{
		cout << endl << "*********************************************************" << endl;
		cout << "                        LLDP                             " << endl;
		cout << "*********************************************************" << endl;
		printf("Dest MAC Address : %02x:%02x:%02x:%02x:%02x:%02x\n",
		(unsigned char) data[0],
		(unsigned char) data[1],
		(unsigned char) data[2],
		(unsigned char) data[3],
		(unsigned char) data[4],
		(unsigned char) data[5]);
		
		printf("Source MAC Address : %02x:%02x:%02x:%02x:%02x:%02x\n",
		(unsigned char) data[6],
		(unsigned char) data[7],
		(unsigned char) data[8],
		(unsigned char) data[9],
		(unsigned char) data[10],
		(unsigned char) data[11]);
		
		printf("Protocol : 0x%02x%02x\n",
		(unsigned char) data[12],
		(unsigned char) data[13]);
		
		for(int i = LLDP_BEFORE; i < size; i++)
		{
			if(type) // zjisteni typu
			{
				type_n = (data[i] >> 1);
				type = false;
			}
			
			if(length)
			{
				length_n = (data[i] << 8) + data[i+1];
				length_n = length_n & 0b111111111;
				i+=2;
				length = false;
			}
			
			switch(type_n)
			{
				case 1:
				{
					cout << "Chassis ID: ";
					
					if(data[i] == 4)
					{
						printf("\tMAC Address : %02x:%02x:%02x:%02x:%02x:%02x\n",
						(unsigned char) data[i+1],
						(unsigned char) data[i+2],
						(unsigned char) data[i+3],
						(unsigned char) data[i+4],
						(unsigned char) data[i+5],
						(unsigned char) data[i+6]);
						i+=6;
					}
					else
					{
						i++;
						for(unsigned int j = 0; j < length_n-1; j++)
						{
							cout << data[i];
							i++;
						}
					}
					
					type = true;
					length = true;
					break;
				}
				case 2:
				{
					cout << "Port-ID: ";
					
					if(data[i] == 3)
					{
						printf("\tMAC Address : %02x:%02x:%02x:%02x:%02x:%02x",
						(unsigned char) data[i+1],
						(unsigned char) data[i+2],
						(unsigned char) data[i+3],
						(unsigned char) data[i+4],
						(unsigned char) data[i+5],
						(unsigned char) data[i+6]);
						i+=7;
					}
					else
					{
						i++;
						for(unsigned int j = 0; j < length_n-1; j++)
						{
							cout << data[i];
							i++;
						}
					}
					
					i--;
					cout << endl;
					type = true;
					length = true;
					break;
				}
				case 3:
				{
					cout << "Time to live: " << (unsigned int)((data[i] << 8) + data[i+1]) << endl;
					i++;
					type = true;
					length = true;
					break;
				}
				case 5:
				{
					cout << "System name: ";
					for(unsigned int j = 0; j < length_n; j++,i++)
					{
						cout << data[i];
					}
					i--;
					cout << endl;
					type = true;
					length = true;
					break;
				}
				case 6:
				{
					cout << "System desription: ";
					for(unsigned int j = 0; j < length_n; j++,i++)
					{
						cout << data[i];
					}
					i--;
					cout << endl;
					type = true;
					length = true;
					break;
				}
				case 4:
				{
					cout << "Port desription: ";
					for(unsigned int j = 0; j < length_n; j++,i++)
					{
						cout << data[i];
					}
					i--;
					cout << endl;
					type = true;
					length = true;
					break;
				}
				case 7:
				{
					bitset<16> capa = (data[i] << 8) + data[i+1];
					i += 2;
					cout << "Capabilities: " << endl;
					cout << "\tCapabilities: " << capa << endl;
					
					if(capa.test(0))
						cout << "\t\t........ .......1 - Other: Capable" << endl;
					else
						cout << "\t\t........ .......0 - Other: Not Capable" << endl;
					
					if(capa.test(1))
						cout << "\t\t........ ......1. - Repeater: Capable" << endl;
					else
						cout << "\t\t........ ......0. - Repeater: Not Capable" << endl;
					
					if(capa.test(2))
						cout << "\t\t........ .....1.. = Bridge: Capable" << endl;
					else
						cout << "\t\t........ .....0.. = Bridge: Not Capable" << endl;
					
					if(capa.test(3))
						cout << "\t\t........ ....1... = WLAN access point: Capable" << endl;
					else
						cout << "\t\t........ ....0... = WLAN access point:: Not Capable" << endl;
					
					if(capa.test(4))
						cout << "\t\t........ ...1.... = Router: Capable"<< endl;
					else
						cout << "\t\t........ ...0.... = Router: Not Capable" << endl;
					
					if(capa.test(5))
						cout << "\t\t........ ..1..... = Telephone: Capable" << endl;
					else
						cout << "\t\t........ ..0..... = Telephone: Not Capable" << endl;
					
					if(capa.test(6))
						cout << "\t\t........ .1...... = DOCSIS cable device: Capable" << endl;
					else
						cout << "\t\t........ .0...... = DOCSIS cable device: Not Capable" << endl;
					
					if(capa.test(7))
						cout << "\t\t........ 1....... = Station only: Capable" << endl;
					else
						cout << "\t\t........ 0....... = Station only: Not Capable" << endl;
					
					capa = (data[i] << 8) + data[i+1];
					
					cout << "\tCapabilities enabled: " << capa << endl;
					
					if(capa.test(0))
						cout << "\t\t........ .......1 - Other: Enabled" << endl;
					else
						cout << "\t\t........ .......0 - Other: Not Enabled" << endl;
					
					if(capa.test(1))
						cout << "\t\t........ ......1. - Repeater: Enabled" << endl;
					else
						cout << "\t\t........ ......0. - Repeater: Not Enabled" << endl;
					
					if(capa.test(2))
						cout << "\t\t........ .....1.. = Bridge: Enabled" << endl;
					else
						cout << "\t\t........ .....0.. = Bridge: Not Enabled" << endl;
					
					if(capa.test(3))
						cout << "\t\t........ ....1... = WLAN access point: Enabled" << endl;
					else
						cout << "\t\t........ ....0... = WLAN access point:: Not Enabled" << endl;
					
					if(capa.test(4))
						cout << "\t\t........ ...1.... = Router: Enabled"<< endl;
					else
						cout << "\t\t........ ...0.... = Router: Not Enabled" << endl;
					
					if(capa.test(5))
						cout << "\t\t........ ..1..... = Telephone: Enabled" << endl;
					else
						cout << "\t\t........ ..0..... = Telephone: Not Enabled" << endl;
					
					if(capa.test(6))
						cout << "\t\t........ .1...... = DOCSIS cable device: Enabled" << endl;
					else
						cout << "\t\t........ .0...... = DOCSIS cable device: Not Enabled" << endl;
					
					if(capa.test(7))
						cout << "\t\t........ 1....... = Station only: Enabled" << endl;
					else
						cout << "\t\t........ 0....... = Station only: Not Enabled" << endl;
					
					i++;
					type = true;
					length = true;
					break;
				}
				case 8:
				{
					cout << "Management Address" << endl;
					cout << "\t Address String Length: " << (unsigned int)data[i] << endl;
					i++;
					if(data[i]==1)
					{
						cout << "\t Address Subtype: IPv4 (1)" << endl;
						i++;
						cout << "\t Management Address: " << (unsigned int)data[i] << "." << (unsigned int)data[i+1] << "." << (unsigned int)data[i+2] << "." << (unsigned int)data[i+3] << endl;
						i+=4;
					}
					else if(data[i]==2)
					{
						cout << "\t Address Subtype: IPv6 (2)" << endl;
						i++;
						printf("\t Management Address: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x\n",
							(int)data[i], (int)data[i+1],
							(int)data[i+2], (int)data[i+3],
							(int)data[i+4], (int)data[i+5],
							(int)data[i+6], (int)data[i+7],
							(int)data[i+8], (int)data[i+9],
							(int)data[i+10], (int)data[i+11],
							(int)data[i+12], (int)data[i+13],
							(int)data[i+14], (int)data[15]);
						i+=16;
							
					}
					
					cout << "\t Interface Subtype: ifIndex (2)" << endl;
					i++;
					cout << "\t Interface Number: " << (unsigned int)((data[i] << 24) + (data[i+1] << 16) + (data[i+2] << 8) + data[i+3]) << endl;
					i+=4;
					cout << "\t OID String Length: " << (unsigned int)data[i] << endl;
					
					type = true;
					length = true;
					break;
				}
				default:
					break;
			}
			
		}
		
	}
	else // tisk pro CDP
	{
		cout << endl << "*********************************************************" << endl;
		cout << "                        CDP                             " << endl;
		cout << "*********************************************************" << endl;
		printf("Dest MAC Address : %02x:%02x:%02x:%02x:%02x:%02x\n",
		(unsigned char) data[0],
		(unsigned char) data[1],
		(unsigned char) data[2],
		(unsigned char) data[3],
		(unsigned char) data[4],
		(unsigned char) data[5]);
		
		printf("Source MAC Address : %02x:%02x:%02x:%02x:%02x:%02x\n",
		(unsigned char) data[6],
		(unsigned char) data[7],
		(unsigned char) data[8],
		(unsigned char) data[9],
		(unsigned char) data[10],
		(unsigned char) data[11]);
		
		unsigned int num = (data[24] << 8) + data[25];
		cout << "Verze: " << (int)data[22] << endl;
		cout << "Time to live: " << (int)data[23] << endl;
		cout << "Checksum: " << num << endl;

		for(int i = 26; i < size; i++)
		{
			if(type)
			{
				type_n = (data[i] << 8) + data[i+1];
				i++;
				i++;
				type = false;
			}
			
			if(length)
			{
				length_n = (data[i] << 8) + data[i+1];
				length_n = length_n - 4;
				i+=2;
				length = false;
			}
			
			switch(type_n)
			{
				case CDP_TYPE_DEVICE_NAME:
				{
					cout << "Device name: ";
					for(unsigned int j = 0; j < length_n; j++,i++)
					{
						cout << data[i];
					}
					i--;
					cout << endl;
					type = true;
					length = true;
					break;
				}
				case CDP_TYPE_DEVICE_ADDRESS:
				{
					cout << "Address: " << endl;
					unsigned int add_count = (data[i] << 24) + (data[i+1] << 16) + (data[i+2] << 8) + data[i+3];
					i += 4;
					cout << "\tAddress count: " << add_count << endl;
					
					for(unsigned int j = 0; j < add_count; j++,i++)
					{
						if(data[i]==1)
						{
							cout << "\t\tProtocol type: NLPID" << endl;
						}
						else if(data[i]==2)
						{
							cout << "\t\tProtocol type: 802.2" << endl;
						}
						i++;
						
						unsigned int pfl = (unsigned int)data[i];
						cout << "\t\tProtocol field length: " << pfl << endl;
						i++;
						if(pfl == 1)
						{
							if(data[i]==0x81)
							{
								cout <<"\t\tProtocol: ISO CLNS" << endl;
								i++;
								i+=2;
							}
							else if(data[i]==0xCC)
							{
								cout <<"\t\tProtocol: IP" << endl;
								i++;
								unsigned int add_length_n = (data[i] << 8) + data[i+1];
								i+=2;
								cout <<"\t\tIP address: ";
								for(unsigned int add_c = 0; add_c < add_length_n; add_c++,i++)
								{
									if(add_c == add_length_n-1)
										cout << (int)data[i] << endl;
									else
										cout << (int)data[i] << ".";
								}
								
							}
						}
						
					}
					i-=2;
					type = true;
					length = true;
					break;
				}
				case CDP_TYPE_DEVICE_PORT_ID:
				{
					cout << "Port-ID: ";
					for(unsigned int j = 0; j < length_n; j++)
					{
						cout << data[i];
						i++;
					}
					i--;
					cout << endl;
					type = true;
					length = true;
					break;
				}
				case CDP_TYPE_DEVICE_CAPABILITIES:
				{
					bitset<32> capa = (data[i] << 24) + (data[i+1] << 16) + (data[i+2] << 8) + data[i+3];
					i = i + 4;
					cout << "Capabilities: " << capa << endl;
					
					if(capa.test(0))
						cout << "\t........ ........ ........ .......1 - Is a Router" << endl;
					else
						cout << "\t........ ........ ........ .......0 - Not a Router" << endl;
					
					if(capa.test(1))
						cout << "\t........ ........ ........ ......1. = Is a Transparent Bridge" << endl;
					else
						cout << "\t........ ........ ........ ......0. = Not a Transparent Bridge" << endl;
					
					if(capa.test(2))
						cout << "\t........ ........ ........ .....1.. = Is a Source Route Bridge" << endl;
					else
						cout << "\t........ ........ ........ .....0.. = Not a Source Route Bridge" << endl;
					
					if(capa.test(3))
						cout << "\t........ ........ ........ ....1... = Is a Switch" << endl;
					else
						cout << "\t........ ........ ........ ....0... = Not a Switch" << endl;
					
					if(capa.test(4))
						cout << "\t........ ........ ........ ...1.... = Is a Host" << endl;
					else
						cout << "\t........ ........ ........ ...0.... = Not a Host" << endl;
					
					if(capa.test(5))
						cout << "\t........ ........ ........ ..1..... = Is IGMP capable" << endl;
					else
						cout << "\t........ ........ ........ ..0..... = Not IGMP capable" << endl;
					
					if(capa.test(6))
						cout << "\t........ ........ ........ .1...... = Is a Repeater" << endl;
					else
						cout << "\t........ ........ ........ .0...... = Not a Repeater" << endl;
					
					
					type = true;
					length = true;
					i--;
					break;
				}
				case CDP_TYPE_DEVICE_VERSION:
					cout << "Version: ";
					for(unsigned int j = 0; j < length_n; j++,i++)
					{
						cout << data[i];
						
					}
					i--;
					cout << endl;
					type = true;
					length = true;
					break;
				case CDP_TYPE_DEVICE_PLATFORM:
					cout << "Platform: ";
					for(unsigned int j = 0; j < length_n; j++,i++)
					{
						cout << data[i];
					}
					i--;
					cout << endl;
					type = true;
					length = true;
					break;
				case CDP_TYPE_DEVICE_PREFIXES:
					cout << "IP prefixes: " << endl;
					if(length_n == 0)
					{
						cout << "\tNONE" << endl;
					}
					else
					{
						for(unsigned int k = 0; k < ((length_n)/5); k++)
						{
							cout << "\t";
							for(unsigned int l = 0; l < 5; l++,i++)
							{
								if(l ==4)
									cout << "/" << (int)data[i] << endl;
								else if (l != 3)
									cout << (int)data[i] << ".";
								else
									cout << (int)data[i];
							}
						}
						i--;
						type = true;
						length = true;
						break;
					}
				case CDP_TYPE_DEVICE_VTP:
				{
					cout << "VTP management domain: ";
					for(unsigned int j = 0; j < length_n; j++,i++)
					{
						cout << data[i];
					}
					i--;
					cout << endl;
					type = true;
					length = true;
					break;
				}
				case CDP_TYPE_DEVICE_VLAN:
				{
					unsigned int native_vlan = (data[i] << 8) + data[i+1];
					cout << "Native VLAN: " << native_vlan << endl;
					i++;
					cout << endl;
					type = true;
					length = true;
					break;
				}
				case CDP_TYPE_DEVICE_DUPLEX:
				{
					if(data[i]==0)
					{
						cout << "Duplex: Half" << endl;
					}
					else
					{
						cout << "Duplex: Full" << endl;
					}
					type = true;
					length = true;
					break;
				}
				default:
					break;
			}
		}
	}
	
}
