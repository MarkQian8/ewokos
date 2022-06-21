#ifndef BASIC_MATH_H
#define BASIC_MATH_H

#include <sys/ewokdef.h>

#ifdef __cplusplus 
extern "C" {
#endif

uint32_t random_u32(void);
uint32_t random_to(uint32_t to);

#ifdef __cplusplus
} 
#endif

#endif
