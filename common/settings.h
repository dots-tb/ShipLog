#define APP_DATA "ux0:data/shipLog/"
#define PLUGIN_DIR "ux0:tai/"

#define KPLUGIN_NAME "backdoor_exe.skprx"
#define NPLUGIN_NAME "shipLog_net.suprx"

#define CONFIG_FILE APP_DATA "config"
#define KPLUGIN_FILE PLUGIN_DIR KPLUGIN_NAME
#define NPLUGIN_FILE PLUGIN_DIR NPLUGIN_NAME

void setIP(const char *ip);
char *getIP();
void setPort(int port);
int getPort();
int isUpdated();
void updated(int bol);
int getLengthOfBuffer();