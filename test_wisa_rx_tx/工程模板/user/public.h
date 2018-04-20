#ifndef _public_H
#define _public_H

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"


void delay(u32 i);

typedef enum {NOEVENT = 0, EVENT_OPCOD_NOTYET_READ = 1,EVENT_OPCOD_READ =2} EventStatus;

#endif
