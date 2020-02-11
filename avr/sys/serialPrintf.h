/***************************************************
 * file: serialPrintf.h
 ***************************************************
 * Use the serial port for debugging.  STDIN and 
 * STDOUT will be mapped to serial functions for 
 * printf() and such
 **************************************************/
#ifndef _SERIAL_PRINTF_
#define _SERIAL_PRINTF_

#include <avr/io.h>

#define     S_BAUD_2400     1
#define     S_BAUD_9600     2
#define     S_BAUD_19200    3
#define     S_BAUD_57600    4
#define     S_BAUD_115200   5

#define     S_FORMAT_8N1    0
#define     S_FORMAT_8E2    1

#define		SERIAL_ENB_RX(x)	if(x){SETBIT(UCSR0B, RXEN0);  }else{ CLRBIT(UCSR0B, RXEN0);}
#define		SERIAL_ENB_TX(x)	if(x){SETBIT(UCSR0B, TXEN0);  }else{ CLRBIT(UCSR0B, TXEN0);}
#define 	SERIAL_ISR_RX(x)	if(x){SETBIT(UCSR0B, RXCIE0); }else{ CLRBIT(UCSR0B, RXCIE0);}
#define 	SERIAL_ISR_TX(x)	if(x){SETBIT(UCSR0B, TXCIE0); }else{ CLRBIT(UCSR0B, TXCIE0);}

bool 	cfgSerial(unsigned char baud, unsigned char format);
int     USART0ReceiveByte(FILE *stream);
int     USART0SendByte(char c, FILE *stream);

#endif // _SERIAL_PRINTF_