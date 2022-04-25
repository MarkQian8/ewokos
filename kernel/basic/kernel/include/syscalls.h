#ifndef SYSCALLS_H
#define SYSCALLS_H

enum {
	SYS_NONE = 0,
	SYS_KPRINT,

	//proccess memory manage
	SYS_MALLOC,
	SYS_REALLOC,
	SYS_FREE,

	SYS_EXEC_ELF,
	SYS_FORK,
	SYS_THREAD,
	SYS_YIELD,
	SYS_WAIT_PID,
	SYS_USLEEP,
	SYS_EXIT,
	SYS_DETACH,
	SYS_BLOCK,
	SYS_WAKEUP,
	SYS_SIGNAL_SETUP,
	SYS_SIGNAL,
	SYS_SIGNAL_END,

	SYS_GET_PID,
	SYS_GET_THREAD_ID,

	SYS_PROC_PING,
	SYS_PROC_READY_PING,

	SYS_PROC_GET_CMD,
	SYS_PROC_SET_CMD,
	SYS_PROC_GET_UID,
	SYS_PROC_SET_UID,

	//share memory syscalls
	SYS_PROC_SHM_ALLOC,
	SYS_PROC_SHM_MAP,
	SYS_PROC_SHM_UNMAP,
	SYS_PROC_SHM_REF,

	SYS_GET_SYS_INFO,
	SYS_GET_SYS_STATE,
	SYS_GET_PROCS,

	SYS_MEM_MAP,
	SYS_KPAGE_MAP, //map kernel memory (just one page size) for userspace access
	SYS_KPAGE_UNMAP, //unmap kernel memory (just one page size) for userspace access

	//internal proccess communication
	SYS_IPC_SETUP,
	SYS_IPC_CALL,
	SYS_IPC_GET_ARG,
	SYS_IPC_SET_RETURN,
	SYS_IPC_GET_RETURN,
	SYS_IPC_END,
	SYS_IPC_DISABLE,
	SYS_IPC_ENABLE,

	SYS_CORE_READY,
	SYS_CORE_PID,
	SYS_GET_KEVENT,
	SYS_GET_KERNEL_TIC,

	SYS_INTR_SETUP,
	SYS_INTR_END
};

#endif
