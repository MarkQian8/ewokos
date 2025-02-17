#include <kernel/kernel.h>
#include <kernel/core.h>
#include <stddef.h>
#include <sconf.h>
#include <atoi.h>

kernel_conf_t _kernel_config;

void load_kernel_config(void) {
	uint32_t cores_max = get_cpu_cores();
	_kernel_config.cores = cores_max; 
	_kernel_config.schedule_freq = SCHEDULE_FREQ;
	_kernel_config.timer_freq = _kernel_config.schedule_freq*2;
	_kernel_config.timer_intr_usec = 0;
	_kernel_config.uart_baud = 115200;

	sconf_t* sconf = sconf_load("/etc/kernel/kernel.conf");
	if(sconf == NULL)
		return;

	const char* v = sconf_get(sconf, "cores");
	if(v[0] != 0)
		_kernel_config.cores = atoi(v);
	if(_kernel_config.cores == 0 || _kernel_config.cores > cores_max)
		_kernel_config.cores = cores_max;

	v = sconf_get(sconf, "timer_freq");
	if(v[0] != 0)
		_kernel_config.timer_freq = atoi(v);

	v = sconf_get(sconf, "schedule_freq");
	if(v[0] != 0)
		_kernel_config.schedule_freq = atoi(v);
	if(_kernel_config.schedule_freq == 0)
		_kernel_config.schedule_freq = SCHEDULE_FREQ;

	if(_kernel_config.timer_freq < _kernel_config.schedule_freq*2)
		_kernel_config.timer_freq = _kernel_config.schedule_freq*2;

	v = sconf_get(sconf, "uart_baud");
	if(v[0] != 0)
		_kernel_config.uart_baud = atoi(v);
	if(_kernel_config.uart_baud == 0)
		_kernel_config.uart_baud = 115200;
	sconf_free(sconf);
}
