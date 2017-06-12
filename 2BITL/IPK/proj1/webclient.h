using namespace std;

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <iterator>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <map>

#define BUFSIZE 4096
#define MAX_REDIRECTS 5
#define HTTP_DEFAULT_1_1 true
#define FILE_NAME_T true
#define FILE_NAME_F false

typedef struct url {
	string path;
	string host;
	int port = 80;
	string file_name;
	string full_link;
} URL;

int get_url(string tmp, url &to, bool file_name_get);
int connect_to(url &to, bool http_1_0);
int get_content(int my_socket, url &to);
string dechunk(string text);
int check_code(string code);
