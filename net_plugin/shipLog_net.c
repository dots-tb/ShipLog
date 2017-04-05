//ShipLog - NetPlugin

//Put together by dots_tb
//Created for Kancolle Kai Vita translation and dev team (expecially you senpai ~<3)
//Special thanks to Team_Molecule for Taihen (special thanks to xyz)
//thanks to xerpi for being underrated (and logging functions/netdebug), frangarcj for oclock
//Freakler for common dialog, TheFlow for VitaShell

//Dialog functions: 
// https://github.com/Freakler/vita-uriCaller

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <taihen.h>

#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/sysmodule.h>

#include <stdlib.h>
#include <backdoor_exe.h>

#include "vita_netdbg.h"
#include "../common/settings.h"
#include "../common/parser.h"

void _start() __attribute__ ((weak, alias ("module_start")));
int watch_log(SceSize args, void *argp) {
	#define BUF_SIZE 1024*16
	char log_buf[BUF_SIZE];
	while(1) {
	//try copying what you need only
	//but for now this works
		int buf_len = shipLogBufLength();
		if(buf_len>0) {
			memset(log_buf,0,BUF_SIZE);
			shipLogcpyBufToUsr(&log_buf,0,BUF_SIZE);
            char buf[512];
            int sent = 0;
            int sending = 0;
            while(sent < buf_len){
                if(buf_len - sent > sizeof(buf) - 1) {
                    sending = sizeof(buf) - 1;
                } else {
                    sending = buf_len - sent;
                }
			
                memcpy(&buf,&log_buf[sent],sending);
                sent += send(buf,sending);
            }
            shipLogBufClear();
		}
		sceKernelDelayThread(1000 * 1000);
	}
	
	return 0;
}
//fix this later
/*int watch_log(SceSize args, void *argp) {
    while(1) {
		int buf_len = shipLogBufLength();
        if(buf_len>0) {
            char buf[512];
            int sent = 0;
            int sending = 0;
            while(sent < buf_len){
                if(buf_len - sent > sizeof(buf) - 1) {
                    sending = sizeof(buf) - 1;
                } else {
                    sending = buf_len - sent;
                }
                shipLogcpyBufToUsr(&buf,sent,sending);
                sent += send(buf,sending);
            }
            shipLogBufClear();
            sceKernelDelayThread(1000 * 1000);
        }
    }
    
    return 0;
}*/

int module_start(SceSize argc, const void *args) {
	printf("Starting ShipLog net_plugin...\n");
	sceSysmoduleLoadModule(SCE_SYSMODULE_NET);
	if(loadConfig(CONFIG_FILE)<0)
		return SCE_KERNEL_START_FAILED;
	
	int ret = init_netdbg(getIP(),getPort());
	if(ret<0)
		return SCE_KERNEL_START_FAILED;
	debug("Put together by dots_tb\n");
	debug("Created for Kancolle Kai Vita translation and dev team (expecially you senpai ~<3)\n");
	debug("Special thanks to Team_Molecule for Taihen (special thanks to xyz)\n");
	debug("thanks to xerpi for being underrated (and logging functions/netdebug), frangarcj for oclock\n");
	debug("Freakler for common dialog, TheFlow for VitaShell\n");
	SceUID thid = sceKernelCreateThread("watch_log", watch_log, 0x40, 0x40000, 0, 0, NULL);
	if (thid >= 0)
		sceKernelStartThread(thid, 0, NULL);
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {
	debug("Bye bye");
	fini_netdbg();
	return SCE_KERNEL_STOP_SUCCESS;
}
