#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ewoksys/vfs.h>
#include <ewoksys/klog.h>
#include <ewoksys/ipc.h>
#include <ewoksys/vdevice.h>
#include <ewoksys/mmio.h>
#include <ewoksys/charbuf.h>
#include <ewoksys/syscall.h>
#include <ewoksys/proc.h>
#include <ewoksys/interrupt.h>
#include <ewoksys/timer.h>

#define KCNTL 0x00
#define KSTAT 0x04
#define KDATA 0x08
#define KCLK  0x0C
#define KISTA 0x10

#define KEYBOARD_BASE (_mmio_base+0x6000)

static uint8_t _held[128] = {0};
static bool _read = false;

int32_t keyb_init(void) {
	_mmio_base = mmio_map();
  put8(KEYBOARD_BASE + KCNTL, 0x10); // bit4=Enable bit0=INT on
  put8(KEYBOARD_BASE + KCLK, 8);
	memset(_held, 0, 128);
	_read = true;
	return 0;
}

//0    1    2    3    4    5    6    7     8    9    A    B    C    D    E    F
const char _ltab[] = {
  0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0, '\t', '`',  0,
  0,   0,   0,   0,   0,  'q', '1',  0,    0,   0,  'z', 's', 'a', 'w', '2',  0,
  0,  'c', 'x', 'd', 'e', '4', '3',  0,    0,  ' ', 'v', 'f', 't', 'r', '5',  0,
  0,  'n', 'b', 'h', 'g', 'y', '6',  0,    0,   0,  'm', 'j', 'u', '7', '8',  0,
  0,  ',', 'k', 'i', 'o', '0', '9',  0,    0,  '.', '/', 'l', ';', 'p', '-',  0,
  0,   0,  '\'', 0,  '[', '=',  0,   0,    0,   0, '\r', ']',  0, '\\',  0,   0,
  0,   0,   0,   0,   0,   0,  '\b', 0,    0,   0,   0,  19,   0,   0,   0,   0,
  0,   0,  24,   0,   4,   5,  27,   0,    0,   0,   0,  0,    0,   0,   0,   0
};

const char _utab[] = {
  0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   '~', 0,
  0,   0,   0,   0,   0,  'Q', '!',  0,    0,   0,  'Z', 'S', 'A', 'W', '@',  0,
  0,  'C', 'X', 'D', 'E', '$', '#',  0,    0,  ' ', 'V', 'F', 'T', 'R', '%',  0,
  0,  'N', 'B', 'H', 'G', 'Y', '^',  0,    0,   0,  'M', 'J', 'U', '&', '*',  0,
  0,  '<', 'K', 'I', 'O', ')', '(',  0,    0,  '>', '?', 'L', ':', 'P', '_',  0,
  0,   0,  '"',  0,  '{', '+',  0,   0,    0,   0,  '\r','}',  0,  '|',  0,   0,
  0,   0,   0,   0,   0,   0,  '\b', 0,    0,   0,   0,  19,   0,   0,   0,   0,
  0,   0,  24,   0,   4,   5,  27,   0,    0,   0,   0,   0,   0,   0,   0,   0
};

static inline uint8_t get_scode(void) {
	return  get8(KEYBOARD_BASE+KDATA);
}

static inline void empty(void) {
	put8(KEYBOARD_BASE+KDATA, 0);
}

#define LSHIFT 0x12
#define RSHIFT 0x59
#define CTRL   0x14

static int32_t keyb_handle(uint8_t scode) {
	if(scode == 0)
		return 0;
	char c = 0;
	//handle release event and key value
	if(scode == 0xF0) { //release event
		scode = get_scode(); // scan released code
		empty(); //set empty data
		if(scode <= 127 && _held[scode] == 1 && scode)  
			_held[scode] = 0;
		return -1; //release 
	}
	else if(scode == 0xFA) //empty data
		return 0;
	
	if(scode > 127)
		return 0;

	_held[scode] = 1;
	if(scode == LSHIFT || scode == RSHIFT || scode == CTRL) 
		return 0;

	if(_held[LSHIFT] == 1 || _held[RSHIFT] == 1) // If shift key held
		c = _utab[scode];
	else 
		c = _ltab[scode];
	return c;
}

static charbuf_t *_buffer;

static int keyb_read(int fd, int from_pid, fsinfo_t* node, 
		void* buf, int size, int offset, void* p) {
	(void)fd;
	(void)from_pid;
	(void)offset;
	(void)p;
	(void)size;
	(void)node;

	char c;
	int res = charbuf_pop(_buffer, &c);

	if(res != 0)
		return VFS_ERR_RETRY;

	((char*)buf)[0] = c;
	return 1;
}

static int loop(void* p) {
	(void)p;
	uint8_t key_scode = get_scode();
	int32_t c = keyb_handle(key_scode);
	if(c != 0) {
		if(c == -1) //release
			c = 0;

		charbuf_push(_buffer, c, true);
		proc_wakeup(RW_BLOCK_EVT);
	}
	proc_usleep(20000);
	return 0;
}

/*static void timer_handler(void) {
	uint8_t key_scode = get8(KEYBOARD_BASE+KDATA);
	char c = keyb_handle(key_scode);
	if(c != 0) {
		charbuf_push(_buffer, c, true);
		proc_wakeup(RW_BLOCK_EVT);
	}
	return;
}
*/

int main(int argc, char** argv) {
	const char* mnt_point = argc > 1 ? argv[1]: "/dev/keyb0";

	keyb_init();
	_buffer = charbuf_new(0);

	vdevice_t dev;
	memset(&dev, 0, sizeof(vdevice_t));
	strcpy(dev.name, "keyb");
	dev.read = keyb_read;
	dev.loop_step = loop;

	//uint32_t tid = timer_set(5000, timer_handler);
	device_run(&dev, mnt_point, FS_TYPE_CHAR, 0444);
	//timer_remove(tid);
	charbuf_free(_buffer);
	return 0;
}
