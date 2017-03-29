//ShipLog - Backdoor.exe

//Put together by dots_tb
//Created for Kancolle Kai Vita translation and decompile team (expecially you senpai ~<3)
//Special thanks to Team_Molecule for Taihen (special thanks to xyz), Mai for MaiDump
//thanks to xerpi for being underrated (and logging functions)
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/threadmgr.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/kernel/suspend.h>

#include <psp2/sysmodule.h>
#include <taihen.h>
#include <psp2kern/io/fcntl.h>

#define DUMP_PATH "ux0:dump/"
#define LOG_FILE DUMP_PATH "logger.txt"

static void log_reset();
static void log_write(const char *buffer, size_t length);

#define LOG(...) \
	do { \
		char buffer[256]; \
		snprintf(buffer, sizeof(buffer), ##__VA_ARGS__); \
		log_write(buffer, strlen(buffer)); \
	} while (0)
static SceUID g_hooks[4];


static int __stdout_fd = -1;
static int __stderr_fd = -1;

static tai_hook_ref_t ref_hook0;
int sceIoWrite_patched(SceUID fd, const void *data, SceSize size) {
	if(fd==__stdout_fd) {
		writeFileUser(fd, "stdout", data, size);
	}
	if(fd==__stderr_fd) {
		writeFileUser(fd, "stderr", data, size);
	}
	return TAI_CONTINUE(int, ref_hook0, fd,data,size);
}
static tai_hook_ref_t ref_hook1;
int sceKernelGetStdout_patched() {
	//debug("sceIoOpen: %s\n",file);
	int fd = TAI_CONTINUE(int, ref_hook1);
	__stdout_fd = fd;
	//LOG("[NOTICE][CAP-STDOUT %d]\n",fd);

	
	return fd;
}
static tai_hook_ref_t ref_hook2;
int sceKernelGetStderr_patched() {
	//debug("sceIoOpen: %s\n",file);
	int fd = TAI_CONTINUE(int, ref_hook2);
	__stderr_fd = fd;
	//LOG("[NOTICE][CAP-STDERR %d]\n",fd);

	
	return fd;
}

void _start() __attribute__ ((weak, alias ("module_start")));

int module_start(SceSize argc, const void *args)
{
	
	log_reset();

	LOG("logger test\n");
	g_hooks[0] = taiHookFunctionExportForKernel(KERNEL_PID,
		&ref_hook0, "SceIofilemgr",0xF2FF276E,
		0x34EFD876, sceIoWrite_patched); 
	g_hooks[1] = taiHookFunctionExportForKernel(KERNEL_PID,
		&ref_hook1, "SceProcessmgr", 0x2DD91812,
		0xE5AA625C,sceKernelGetStdout_patched);
	g_hooks[2] = taiHookFunctionExportForKernel(KERNEL_PID,
		&ref_hook2, "SceProcessmgr", 0x2DD91812,
		0xFA5E3ADA,sceKernelGetStderr_patched);
	LOG("hooks:\n");
	LOG("hooks[sceIoWrite]: %d\n", g_hooks[0]);
	LOG("hooks[sceKernelGetStdout]: %d\n", g_hooks[1]);
	LOG("hooks[sceKernelGetStderr]: %d\n", g_hooks[2]);
	return SCE_KERNEL_START_SUCCESS;

}

int module_stop(SceSize argc, const void *args)
{
 if (g_hooks[0] >= 0) taiHookReleaseForKernel(g_hooks[0], ref_hook0);
 if (g_hooks[1] >= 0) taiHookReleaseForKernel(g_hooks[1], ref_hook1);
 if (g_hooks[2] >= 0) taiHookReleaseForKernel(g_hooks[2], ref_hook2);
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
void writeFileUser(int name_fd, const char * name, const void *buffer, size_t length)
{
	char path[128];
	snprintf(path,sizeof(path),"%s%s-%d.txt",DUMP_PATH,name,name_fd);
	extern int ksceIoMkdir(const char *, int);
	ksceIoMkdir(DUMP_PATH, 6);

	SceUID fd = ksceIoOpen(path,
		SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 6);
	if (fd < 0)
		return;
	char buf[256];
	int sent = 0;
	int sending = 0;
	while(sent<length) {
		if(0>length - 256) {
			sending = 256;
		} else {
			sending = length - sent;
		}
		ksceKernelMemcpyUserToKernel(&buf, (uintptr_t)(buffer + sent),sending);
		sent += ksceIoWrite(fd, buf, sending);
		
	}
	
	ksceIoClose(fd);
}
