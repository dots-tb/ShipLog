int init_netdbg(const char *ip,int port);
void fini_netdbg();
int debug(char *text, ...);
int send(char *text,int len);
int get_socket();