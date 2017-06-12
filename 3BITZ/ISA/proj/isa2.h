/*Author: Jan Pavlica, xpavli78, VUT FIT
Description: This packet sniffer can analyze CDP and LLDP packets.
             It can also sends CDP and LLDP packets on specified interface.*/

#include <iostream>
#include <iomanip>    
#include <stdio.h>
#include <pcap.h>
#include <bitset>
#include <time.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <getopt.h>

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <stdint.h>

#define MY_DEST_MAC0	0x01
#define MY_DEST_MAC1	0x80
#define MY_DEST_MAC2	0xc2
#define MY_DEST_MAC3	0x00
#define MY_DEST_MAC4	0x00
#define MY_DEST_MAC5	0x0e

#define MY_DEST_MAC0_CDP	0x01
#define MY_DEST_MAC1_CDP	0x00
#define MY_DEST_MAC2_CDP	0x0c
#define MY_DEST_MAC3_CDP	0xcc
#define MY_DEST_MAC4_CDP	0xcc
#define MY_DEST_MAC5_CDP	0xcc

#define TTL_DEFAULT 180
#define DUPLEX_HALF 0
#define DUPLEX_FULL 1
#define HOSTNAME_SIZE 128
#define CDP_CAPABILITIES 16
#define LLDP_CAPABILITIES 128

#define LLDP_TYPE_PORT_ID 2
#define LLDP_TYPE_TTL 3
#define LLDP_SIZE_TTL 2
#define LLDP_TYPE_SYSTEM_NAME 5
#define LLDP_TYPE_SYSTEM_DESCRIPTION 6
#define LLDP_TYPE_CAPABILITIES 7
#define LLDP_SIZE_CAPABILITIES 4
#define LLDP_TYPE_MANAGEMENT_ADDRESS 8
#define LLDP_SIZE_MANAGEMENT_ADDRESS 12

#define CDP_TYPE_DEVICE_NAME 1
#define CDP_TYPE_DEVICE_ADDRESS 2
#define CDP_TYPE_DEVICE_PORT_ID 3
#define CDP_TYPE_DEVICE_CAPABILITIES 4
#define CDP_TYPE_DEVICE_VERSION 5
#define CDP_TYPE_DEVICE_PLATFORM 6
#define CDP_TYPE_DEVICE_PREFIXES 7
#define CDP_TYPE_DEVICE_VTP 9
#define CDP_TYPE_DEVICE_VLAN 10
#define CDP_TYPE_DEVICE_DUPLEX 11

#define CDP_BEFORE 22
#define LLDP_BEFORE 14

#define BUF_SIZ		1024

using namespace std;

typedef struct tlvs{
	char *dev;
	bool hello;
	int ttl;
	int duplex;
	string software_version;
	char *device_id;
	char *platform;
	char *port_id;
	int capabilities_cdp;
	int capabilities_lldp;
	char *ipv4;
	int interface_number;
} tlvs;

/* funkce pro složení TLV halvičky u LLDP */
bitset <16> concat( const bitset <7> & b1, const bitset <9> & b2 ) {
    string s1 = b1.to_string();
    string s2 = b2.to_string();
    return bitset <16>( s1 + s2 );
}
