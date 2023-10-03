#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <sys/vfs.h>
#include <vprintf.h>
#include <sysinfo.h>
#include <sys/klog.h>
#include <sys/ipc.h>
#include <sys/proc.h>
#include <dirent.h>
#include <sd/sd.h>
#include <ext2/ext2fs.h>
#include <bsp/bsp_sd.h>

static void outc(char c, void* p) {
	str_t *buf = (str_t *)p;
	str_addc(buf, c);
}

static void out(const char *format, ...) {
	str_t *str = str_new(NULL);
	va_list ap;
	va_start(ap, format);
	v_printf(outc, str, format, ap);
	va_end(ap);
	klog("%s", str->cstr);
	str_free(str);
}

static void* sd_read_ext2(const char* fname, int32_t* size) {
	ext2_t ext2;
	ext2_init(&ext2, sd_read, NULL);
	void* ret = ext2_readfile(&ext2, fname, size);
	ext2_quit(&ext2);
	return ret;
}

static int32_t exec_from_sd(const char* prog) {
	int32_t sz;
	if(bsp_sd_init() != 0){
		printf("bsp init failed\n");
		return -1;
	}

	char* elf = sd_read_ext2(prog, &sz);
	if(elf != NULL) {
		int res = syscall3(SYS_EXEC_ELF, (int32_t)prog, (int32_t)elf, sz);
		free(elf);
		if(res == 0) {
			return res;
		}
	}
	return -1;
}

static void run_before_vfs(const char* cmd) {
	out("init: %s    ", cmd);

	int pid = fork();
	if(pid == 0) {
		if(exec_from_sd(cmd) != 0) {
			out("[failed]!\n");
			exit(-1);
		}
	}
	else
		proc_wait_ready(pid);
	out("[ok]\n");
}

static const char* get_arch_initrd(void) {
	static char rd[FS_FULL_NAME_MAX] = "";
	sys_info_t sysinfo;
	syscall1(SYS_GET_SYS_INFO, (int32_t)&sysinfo);
	snprintf(rd, FS_FULL_NAME_MAX-1, "/etc/arch/%s/init.rd", sysinfo.machine);
	return rd;
}

static void run_init(const char* init_file) {
	if(vfs_access(init_file) != 0) {
		out("init: init file '%s' missed! \n", init_file);
		return;
	}

	int pid = fork();
	if(pid == 0) {
		setuid(0);
		char cmd[FS_FULL_NAME_MAX];
		snprintf(cmd, FS_FULL_NAME_MAX-1, "/bin/shell -initrd %s", init_file);
		out("\ninit: loading '%s' ... \n", init_file);
		if(exec(cmd) != 0) {
			out("[failed]!\n");
			exit(-1);
		}
	}
	else 
		waitpid(pid);
}

static void init_stdio(void) {
	int fd = open("/dev/tty0", 0);
	int fd_console = open("/dev/console0", 0);

	if(fd > 0) {
		dup2(fd, 0);
		dup2(fd, 1);
		close(fd);
	}
	if(fd_console > 0) {
		dup2(fd_console, 2);
		close(fd_console);
	}
}

static void switch_root(void) {
	run_init("/etc/init.rd");
	run_init("/etc/arch.rd");
	run_init("/etc/system.rd");
	init_stdio();
	run_init("/etc/profile.rd");
}

static void halt(void) {
	while(1);
}

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	if(getuid() >= 0) {
		out("process 'init' can only loaded by kernel!\n");
		return -1;
	}

	if(getpid() != 0) //not first proc, must be cpu core idle
		halt();
	else
		syscall1(SYS_PROC_SET_CMD, (int32_t)"/sbin/init");

	out("\n[init process started]\n");
	run_before_vfs("/sbin/core");
	run_before_vfs("/sbin/vfsd");
	run_before_vfs("/sbin/sdfsd");

	switch_root();
	while(true) {
		proc_block(getpid(), (uint32_t)main);
	}
	return 0;
}
