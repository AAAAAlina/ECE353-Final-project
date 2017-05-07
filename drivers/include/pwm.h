#ifndef __PWM_H__
#define __PWM_H__

#include <stdint.h>
#include <stdbool.h>
#include "TM4C123GH6PM.h"


#define PWM_ENABLE_PWM0A          (1 << 0)
#define PWM_ENABLE_PWM0B          (1 << 1)
#define PWM_ENABLE_PWM1A          (1 << 2)
#define PWM_ENABLE_PWM1B          (1 << 3)
#define PWM_ENABLE_PWM2A          (1 << 4)
#define PWM_ENABLE_PWM2B          (1 << 5)
#define PWM_ENABLE_PWM3A          (1 << 6)
#define PWM_ENABLE_PWM3B          (1 << 7)

#define PWM3GENA 									0x0000008C
#define PWM3GENB 									0x0000080C
#define PWM3LOAD									0x0000018F
#define PWM3CMPA									0x0000012B
#define PWM3CMPB									0x00000063

#define SYSCTL_RCC_USEPWMDIV			0x00100000  //Enable PWM divide
#define GPIO_PCTL_M1PWM6					0x00000500	//Port Control M1PWM6

uint8_t pwmConfig(
  uint32_t gena,
  uint32_t genb
);

void pwmEnable(uint16_t count);
void pwmDisable(void);
  
#endif
