#ifndef _PROJECT_H_INCLUDED
#define _PROJECT_H_INCLUDED

/*******************************************
* Project globals
*******************************************/
#include <stdint.h>
extern uint32_t milliseconds;
//typedef uint8_t bool;

/*********************************************
* System and std lib includes and definitions
*********************************************/
#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/*********************************************
* Project includes and definitions
**********************************************/
#include "./sys/hardware.h"
#include "./sys/eeprom.h"
#include "./sys/serialPrintf.h"

#include "./sys/ioDefinitions.h"

/*********************************************
* Number of tasks, and by definition, interval
* in MS between task iterations
**********************************************/
#define TASK_INTERVAL   4

#define true    1
#define false   0

#ifndef BIT
#define BIT(x) 1U << x
#endif

#ifndef SETBIT
#define	SETBIT(a, b)	(a |= BIT(b))
#endif

#ifndef GETBIT
#define	GETBIT(a, b)	(a & BIT(b))
#endif

#ifndef CLRBIT
#define CLRBIT(a, b)	(a &= ~BIT(b))
#endif

#define DBGALL      1

#ifdef  DBGALL
#define DBGPRINT        printf_P(PSTR
#else
#define DBGPRINT
#endif 

#define printflash(s)   printf_P(PSTR(s))

#define IO_PORT_2_7     PORTD
#define IO_PORT_DDR_2_7 DDRD
#define IO_PORT_PIN_2_7 PIND

#define IO_PORT_8_9     PORTB
#define IO_PORT_DDR_8_9 DDRB
#define IO_PORT_PIN_8_9 PINB

#define IO_PIN_2        PIND2
#define IO_PIN_3        PIND3
#define IO_PIN_4        PIND4
#define IO_PIN_5        PIND5
#define IO_PIN_6        PIND6
#define IO_PIN_7        PIND7
#define IO_PIN_8        PINB0
#define IO_PIN_9        PINB1

#define DEBUG_PIN_0(x)  if(x){SETBIT(PORTD, PD6);}else{CLRBIT(PORTD, PD6);}
#define DEBUG_PIN_1(x)  if(x){SETBIT(PORTD, PD7);}else{CLRBIT(PORTD, PD7);}

#endif // _PROJECT_H_INCLUDED
