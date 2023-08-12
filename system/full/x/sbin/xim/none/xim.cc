#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/vdevice.h>
#include <sys/klog.h>
#include <sys/proc.h>
#include <sys/keydef.h>
#include <sys/kernel_tic.h>
#include <x/xwin.h>
#include <string.h>
#include <sys/timer.h>

#define KEY_REPEAT_TIMEOUT	80
#define KEY_HOLD_TIMEOUT	200
#define KEY_TIMER	        20000 //50 ps

typedef struct {
	uint8_t key;
	int state;
	int sm;
	uint64_t timer;
}KeyState_t;

enum {
	KS_IDLE = 0,
	KS_HOLD,
	KS_REPEAT,
	KS_RELEASE
};

class XIM {
	int x_pid;
	int keybFD;
	bool escHome;
	KeyState_t keyState[6];

	KeyState_t* match_key(uint8_t key){
		for(size_t i = 0; i < sizeof(keyState)/sizeof(KeyState_t); i++){
			if(keyState[i].key == key){
				return &keyState[i];
			}
		}

		for(size_t i = 0; i < sizeof(keyState)/sizeof(KeyState_t); i++){
			if(keyState[i].key == 0){
				keyState[i].key = key;
				keyState[i].sm = KS_IDLE;
				keyState[i].timer = 0;
				keyState[i].state = XIM_STATE_PRESS;
				return &keyState[i]; 
			}
		}
		return NULL;
	}

	void update_key_state(char *keys, int size){
		for(int i = 0; i < size ; i++){
			KeyState_t *ks = match_key(keys[i]);
			if(ks){
				ks->state = XIM_STATE_PRESS; 
			}
		}		
	}

	void key_state_machine(){
		for(size_t i = 0; i < sizeof(keyState)/sizeof(KeyState_t); i++){
			KeyState_t *ks = &keyState[i];
			
			if(ks->key == 0)
				continue;

			if(ks->state == XIM_STATE_RELEASE){
				input(ks->key, XIM_STATE_RELEASE);	
				ks->key = 0;
				ks->sm = KS_RELEASE;
				continue;
			}

			switch(ks->sm){
				case KS_IDLE:
					input(ks->key, XIM_STATE_PRESS);
					ks->sm = KS_HOLD;
					ks->timer = kernel_tic_ms(0);
					break;
				case KS_HOLD:
					if(kernel_tic_ms(0) - ks->timer > KEY_HOLD_TIMEOUT){
						ks->timer = kernel_tic_ms(0);
						ks->sm = KS_REPEAT;	
					}
					break;
				case KS_REPEAT:
					if(kernel_tic_ms(0) - ks->timer > KEY_REPEAT_TIMEOUT){
						ks->timer = kernel_tic_ms(0);
						input(ks->key, XIM_STATE_PRESS);	
					}
					break;
				default:
					ks->sm = KS_IDLE;
					break;
			}
			ks->state = XIM_STATE_RELEASE;
		}	
	}

	void input(char c, int state = XIM_STATE_PRESS) {
		xevent_t ev;
		ev.type = XEVT_IM;
		if((c == KEY_ESC || c == KEY_BUTTON_SELECT) && escHome)
			c = KEY_HOME;
		ev.value.im.value = c;
		ev.state = state;

		proto_t in;
		PF->init(&in)->add(&in, &ev, sizeof(xevent_t));
		dev_cntl_by_pid(x_pid, X_DCNTL_INPUT, &in, NULL);
		PF->clear(&in);
	}


public:
	inline XIM(const char* keyb_dev, bool escHome) {
		x_pid = -1;
		keybFD = -1;
		this->escHome = escHome;
		while(true) {
			keybFD = open(keyb_dev, O_RDONLY);
			if(keybFD > 0)
				break;
			usleep(300000);
		}
	}

	inline ~XIM() {
		if(keybFD < 0)
			return;
		::close(keybFD);
	}

	void read(void) {
		if(x_pid < 0)
			x_pid = dev_get_pid("/dev/x");
		if(x_pid <= 0 || keybFD < 0)
			return;

		char v[6];
		int rd = ::read(keybFD, &v, 6);
		update_key_state(v, rd);
		key_state_machine();
		//usleep(50000);
	}
};

static XIM* _xim = NULL;

static void timer_handler(void) {
	_xim->read();
}

int main(int argc, char* argv[]) {
	const char* keyb_dev = "/dev/keyb0";
	if(argc > 1)
		keyb_dev = argv[1];

	bool escHome = false;
	if(argc > 2 && strcmp(argv[2], "esc_home") == 0) {
		escHome = true;
	}

	XIM xim(keyb_dev, escHome);
	_xim = &xim;
	
	uint32_t tid = timer_set(KEY_TIMER, timer_handler);

	while(true) {
		proc_block(getpid(), (uint32_t) main);
	}

	timer_remove(tid);
	return 0;
}
