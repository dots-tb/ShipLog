//ShipLog - LaunchPad

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
#include <stdlib.h>

#include <psp2/kernel/clib.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/threadmgr.h>

#include "blit.h"
#include "../common/settings.h"
#include "../common/parser.h"

static SceUID g_hooks[4];
static SceUID mod_id = -1;
static int screen = 1;
static char string[512];
SceUID starter_thid = -1;
int net_start(SceSize args, void *argp) {
	if(loadConfig(CONFIG_FILE)>=0){
		for(int i = 5; i>=0; i--){
			sprintf(string,"Will connect to %s:%i... %d",getIP(),getPort(),i);
			sceKernelDelayThread(1 * 1000 * 1000);
		}
		mod_id = sceKernelLoadStartModule(NPLUGIN_FILE, 0, NULL, 0, NULL, NULL);
		if(mod_id > 0) {
			sprintf(string,"Success: %d",mod_id);
			sceKernelDelayThread(5 * 1000 * 1000);
			screen = 0;
		} 
		sprintf(string,"shipLog_net ret %d",mod_id);
	} else {
		sprintf(string,"ERROR CONFIG NOT FOUND");
		sceKernelDelayThread(5 * 1000 * 1000);
		screen = 0;
	}
	return 0;
}

static tai_hook_ref_t ref_hook1;
int sceDisplaySetFrameBuf_patched(const SceDisplayFrameBuf *pParam, int sync) {
	if(starter_thid < 0) {
		starter_thid = sceKernelCreateThread("_starter", net_start, 0x40, 0x40000, 0, 0, NULL);
		if (starter_thid >= 0)
			sceKernelStartThread(starter_thid, 0, NULL);
	}
	if(screen) {
		blit_set_frame_buf(pParam);
		blit_set_color(0x00FFFFFF, 0x00FF0000);
		blit_string(10, 10,"ShipLog IS RUNNING");
		blit_string(10, 30,string);
	}
	return TAI_CONTINUE(int, ref_hook1, pParam, sync);
}

void _start() __attribute__ ((weak, alias ("module_start")));

int module_start(SceSize argc, const void *args) {
	g_hooks[1] = taiHookFunctionImport(&ref_hook1, 
		TAI_MAIN_MODULE,
		TAI_ANY_LIBRARY,
		0x7A410B64, // sceDisplaySetFrameBuf
		sceDisplaySetFrameBuf_patched);

	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {
	if (g_hooks[1] >= 0) taiHookRelease(g_hooks[1], ref_hook1);
	return SCE_KERNEL_STOP_SUCCESS;
}
