//https://gist.github.com/xerpi/e426284df19c217a8128
//XERPRI THE HIDDEN GEM
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "vita_netdbg.h"

#include <psp2/net/net.h>
#include <psp2/net/netctl.h>

static int _dbgsock = 0;
static void *net_memory = NULL;

int init_netdbg(const char *ip,int port)
{
	#define NET_INIT_SIZE 1*1024*1024
	SceNetSockaddrIn server;
	server.sin_len = sizeof(server);
	server.sin_family = SCE_NET_AF_INET;
	
	sceNetInetPton(SCE_NET_AF_INET, ip, &server.sin_addr);
	server.sin_port = sceNetHtons(port);
	memset(server.sin_zero, 0, sizeof(server.sin_zero));
	

	_dbgsock = sceNetSocket("vitanetdbg", SCE_NET_AF_INET, SCE_NET_SOCK_STREAM, 0);
	int ret = sceNetConnect(_dbgsock, (SceNetSockaddr *)&server, sizeof(server));

	if(ret < 0)
		return ret;
	else
		debug("Hello from Vita");
	return 0;
}
int get_socket() {
	return _dbgsock;
}
void fini_netdbg()
{
	if (_dbgsock) {
		sceNetSocketClose(_dbgsock);
		sceNetTerm();
		if (net_memory) {
			free(net_memory);
			net_memory = NULL;
		}
		_dbgsock = 0;
	}
}

int debug(char *text, ...)
{
	va_list list;
	char string[512];

	va_start(list, text);
	vsprintf(string, text, list);
	va_end(list);

	return sceNetSend(_dbgsock, string, strlen(string), 0);
}
int send(char *text,int len) {
	return sceNetSend(_dbgsock, text, len, 0);
}