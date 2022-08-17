#include <kernel/irq.h>
#include <kernel/kernel.h>
#include <kernel/hw_info.h>
#include "timer_arch.h"
#include <gic.h>

void irq_arch_init(void) {
	gic_init(MMIO_BASE + 0x138000);
}

inline uint32_t irq_gets(void) {
	int ack = gic_get_irq();
	int irqno = ack & 0x3FF;
	int core = get_core_id();//ack & (~0x3FF);
	//if(irqno == 0)
	//	printf("%s %d %d\n", __func__, core, irqno);
	if(irqno == 27){
		return IRQ_TIMER0;
	}else if(irqno == 0){
		return IRQ_IPI;
	}
	return 0;
}

inline void irq_enable(uint32_t irqs) {
	(void)irqs;
	if(irqs| IRQ_TIMER0)
		gic_irq_enable(0, 27);
}

void irq_disable(uint32_t irqs) {
	(void)irqs;
}
