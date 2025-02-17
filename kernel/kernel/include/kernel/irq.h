#ifndef IRQ_H
#define IRQ_H

#include <kernel/context.h>

#define IRQ_TIMER0 0x00000001
#define IRQ_TIMER1 0x00000002
#define IRQ_TIMER2 0x00000004
#define IRQ_TIMER3 0x00000008

#define IRQ_UART0  0x00000010
#define IRQ_UART1  0x00000020
#define IRQ_UART2  0x00000040
#define IRQ_UART3  0x00000080

#define IRQ_KEY    0x00000200
#define IRQ_MOUSE  0x00000400
#define IRQ_SDC    0x00000800

#define IRQ_IPI    0x00001000

extern void dump_ctx(context_t *ctx);
extern void irq_handler(context_t* ctx);
extern void prefetch_abort_handler(context_t* ctx, uint32_t status);
extern void data_abort_handler(context_t* ctx, uint32_t addr_fault, uint32_t status);
extern void irq_init(void);
extern void irq_arch_init(void);
extern void irq_enable(uint32_t irq);
extern void irq_disable(uint32_t irq);
extern void irq_enable_cpsr(context_t* ctx);
extern void irq_disable_cpsr(context_t* ctx);
extern uint32_t irq_gets(void);

#endif
