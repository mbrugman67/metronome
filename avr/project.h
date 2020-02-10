#ifndef _PROJECT_H_INCLUDED
#define _PROJECT_H_INCLUDED

/*******************************************
* Project globals
*******************************************/
#include <stdint.h>
extern uint32_t milliseconds;

/*********************************************
* Number of LEDS in the metronome string
**********************************************/
#define STRING_LENGTH   10

/*********************************************
* Number of tasks, and by definition, interval
* in MS between task iterations
**********************************************/
#define TASK_INTERVAL   4

#define true    1
#define false   0

#ifndef BIT
#define BIT(x) (1u << x)
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

#endif // _PROJECT_H_INCLUDED
