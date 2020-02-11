/***************************************************
 * file: serialPrintf.cpp
 ***************************************************
 * Use the serial port for debugging.  STDIN and 
 * STDOUT will be mapped to serial functions for 
 * printf() and such
 **************************************************/
#include <stdlib.h>
#include <stdio.h>

#include "../project.h"
#include "serialPrintf.h"

/***************************************************
 * USART0SendByte()
 ***************************************************
 * Send a single byte out the serial port.  
 * There is a reference to a FILE pointer; this is
 * needed to use this function for STDOUT
 **************************************************/
int USART0SendByte(char c, FILE *stream)
{
    while(!GETBIT(UCSR0A, UDRE0));
    UDR0 = c;
    
    return 0;
}

/***************************************************
 * USART0ReceiveByte()
 ***************************************************
 * Read a single byte in from the serial port.  
 * There is a reference to a FILE pointer; this is
 * needed to use this function for STDIN
 **************************************************/
int USART0ReceiveByte(FILE *stream)
{
    uint8_t c;
    // Wait for byte to be received
    while(!GETBIT(UCSR0A, RXC0));
    c = UDR0;
    //echo input data
    USART0SendByte(c, stream);
    // Return received data
    return c;
}

/********************************************
* cfgSerial()
*********************************************
* Set baud rate and data format, get pins
* set up and rock on
********************************************/
bool cfgSerial(unsigned char baud, unsigned char format)
{
    // start by turning the serial port off
    UCSR0B = 0;
	UCSR0C = 0;

	// set baud rate doubler for more accuracy
	SETBIT(UCSR0A, U2X0);

    switch(baud)
    {
        case S_BAUD_2400:
        {
            UBRR0 = 832;    // error 0.0%

        } break;

        case S_BAUD_9600:
        {
            UBRR0 = 207;    // error 0.2%

        } break;

        case S_BAUD_19200:
        {
            UBRR0 = 103;   	// error 0.2%
        } break;

        case S_BAUD_57600:
        {
            UBRR0 = 34;    	// error -0.8%
        } break;

        case S_BAUD_115200:
        {
            UBRR0 = 16;    	// error 2.1%
        } break;

        default:
            return false;
    }

	switch(format)
	{
		case S_FORMAT_8N1:
		{
			SETBIT(UCSR0C, UCSZ01);
			SETBIT(UCSR0C, UCSZ00);
		} break;

		case S_FORMAT_8E2:
		{
			SETBIT(UCSR0C, UCSZ01);
			SETBIT(UCSR0C, UCSZ00);
			SETBIT(UCSR0B, USBS0);
		} break;

		default:
			return false;
	}
    
    SERIAL_ENB_TX(true);
    SERIAL_ENB_RX(true);
        
    return true;
}
