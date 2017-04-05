//ShipLog - Backdoor.exe

//Put together by dots_tb
//Created for Kancolle Kai Vita translation and dev team (expecially you senpai ~<3)
//Special thanks to Team_Molecule for Taihen (special thanks to xyz)
//thanks to xerpi for being underrated (and logging functions/netdebug), frangarcj for oclock
//Freakler for common dialog, TheFlow for VitaShell

//Dialog functions: 
// https://github.com/Freakler/vita-uriCaller

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/threadmgr.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/kernel/cpu.h>
#include <psp2kern/io/fcntl.h>
#include <psp2kern/net/net.h>
#include <psp2kern/io/fcntl.h>
#include <taihen.h>

#include "backdoor_exe.h"

#define DUMP_PATH "ux0:dump/"
#define LOG_FILE DUMP_PATH "logger.txt"
#define BUF_SIZE 1024*16

static int __stdout_fd = 1073807367;
static char buffer[BUF_SIZE];
static int buf_pos = 0;
static SceUID g_hooks[2];

static void log_write(const char *buffer, size_t length);
void copyFromUserToBuf(const void *buf, size_t length);

#define LOG(...) \
	do { \
		char buffer[256]; \
		snprintf(buffer, sizeof(buffer), ##__VA_ARGS__); \
		log_write(buffer, strlen(buffer)); \
	} while (0)

static tai_hook_ref_t ref_hook0;
int sceIoWrite_patched(SceUID fd, const void *data, SceSize size) {
	if(fd==__stdout_fd) {
		copyFromUserToBuf(data, size);
		shipLogWriteCB(data, size);
	}
	return TAI_CONTINUE(int, ref_hook0, fd,data,size);
}
static tai_hook_ref_t ref_hook1;
int sceKernelGetStdout_patched() {
	//debug("sceIoOpen: %s\n",file);
	int fd = TAI_CONTINUE(int, ref_hook1);
	__stdout_fd = fd;

	
	return fd;
}


void _start() __attribute__ ((weak, alias ("module_start")));

int module_start(SceSize argc, const void *args)
{
	

	LOG("ShipLog\n");
	g_hooks[0] = taiHookFunctionExportForKernel(KERNEL_PID,
		&ref_hook0, "SceIofilemgr",0xF2FF276E,
		0x34EFD876, sceIoWrite_patched); 
	g_hooks[1] = taiHookFunctionExportForKernel(KERNEL_PID,
		&ref_hook1, "SceProcessmgr", 0x2DD91812,
		0xE5AA625C,sceKernelGetStdout_patched);

	LOG("hooks:\n");
	LOG("hooks[sceIoWrite]: %d\n", g_hooks[0]);
	LOG("hooks[sceKernelGetStdout]: %d\n", g_hooks[1]);
	return SCE_KERNEL_START_SUCCESS;

}

int module_stop(SceSize argc, const void *args)
{
 if (g_hooks[0] >= 0) taiHookReleaseForKernel(g_hooks[0], ref_hook0);
 if (g_hooks[1] >= 0) taiHookReleaseForKernel(g_hooks[1], ref_hook1);

	return SCE_KERNEL_STOP_SUCCESS;
}
void log_reset()
{
	SceUID fd = ksceIoOpen(LOG_FILE,
		SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 6);
	if (fd < 0)
		return;

	ksceIoClose(fd);
}

void log_write(const char *buffer, size_t length)
{
	extern int ksceIoMkdir(const char *, int);
	ksceIoMkdir(DUMP_PATH, 6);

	SceUID fd = ksceIoOpen(LOG_FILE,
		SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 6);
	if (fd < 0)
		return;

	ksceIoWrite(fd, buffer, length);
	ksceIoClose(fd);
}
void copyFromUserToBuf(const void *buf, size_t length)
{
	int sent = 0;
	int sending = 0;
	while(sent<length) {
		if(BUF_SIZE - buf_pos > length - sent) {
			sending = length - sent;
		} else {
			sending = BUF_SIZE - buf_pos;
		}
		ksceKernelMemcpyUserToKernel(&buffer[buf_pos], (uintptr_t)(buf + sent),sending);
		if(buf_pos >= BUF_SIZE) {
			log_write(buffer, BUF_SIZE);
			memset(buffer,0,BUF_SIZE);
			buf_pos = 0;
		} else	
			buf_pos += sending;
		sent = sending;
	}
}

int shipLogBufLength() {
	int state;
	int ret;
	ENTER_SYSCALL(state);
	ret = buf_pos;
	EXIT_SYSCALL(state);
	return ret;
}
//not sure how secure this is
int shipLogcpyBufToUsr(const void *dest, int pos, int length) {
	int state;
	ENTER_SYSCALL(state);
	if((BUF_SIZE >= pos + length)&&
		(pos >= 0) && (length >= 0))
		ksceKernelMemcpyKernelToUser((uintptr_t)dest, buffer + pos,length);
		EXIT_SYSCALL(state);
		return length;
	EXIT_SYSCALL(state);
	return -1;
	
}
void shipLogBufClear() {
	int state;
	ENTER_SYSCALL(state);
	memset(buffer,0,BUF_SIZE);
	buf_pos = 0;
	EXIT_SYSCALL(state);
}
int shipLogWriteCB(const void *buf, int size) {
}
void shipLogDumpToDisk() {
	int state;
	ENTER_SYSCALL(state);
	log_write(buffer, buf_pos);
	//probably should use the function above.
	memset(buffer,0,BUF_SIZE);
	buf_pos = 0;
	EXIT_SYSCALL(state);
}