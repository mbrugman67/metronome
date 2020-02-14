/***************************************************
 * file: lcd.cpp
 ***************************************************
 * handle standard Hitachi-interfaced LCD.  This
 * particular unit is a 4x20 and will be used in
 * 4-bit mode
 **************************************************/

#include <stdlib.h>
#include <string.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lcd.h"
#include "../sys/ioDefinitions.h"

volatile uint8_t backlightPWMVal;

// Really think before enabling DEBUG here, it will
// screw up the timing of the signals...
#ifdef DEBUG
#undef DEBUG
#endif

#ifdef DEBUG
#include <stdio.h>
#include <avr/pgmspace.h>
#endif

// 4 x 20 display
#define MAX_LINE_LENGTH     20

lcd* lcd::_inst = NULL;

/************************************************
 * getInstance()
 ************************************************
 * this class is a singleton, get the one instance
 * and if it doesn't exist yet, create and init it
 ***********************************************/
lcd* lcd::getInstance()
{
    if (!_inst)
    {
        _inst = (lcd*)malloc(sizeof(lcd));
        backlightPWMVal = 24;
        _inst->init();
    }

    return (_inst);
}

/************************************************
 * clearAll()
 ************************************************
 * command 0x01 will clear the display and send
 * the address pointer to top left.  This takes
 * about 1.5 ms to do...
 ***********************************************/
void lcd::clearAll()
{
    this->sendCmd(0x01); // clear the whole thang
    _delay_ms(2);     // This command takes 1.64 ms

#ifdef DEBUG
    printf_P(PSTR("lcd::clearAll()\n"));
#endif
}

/************************************************
 * clearLine()
 ************************************************
 * clear the specified line by writing spaces
 * across all chars in that line
 ***********************************************/
void lcd::clearLine(lcd_line_t line)
{
    for (size_t ii = 0; ii < MAX_LINE_LENGTH; ii++)
    {
        this->sendCmd((uint8_t)line + ii);
        this->sendChar(' ');
    }

#ifdef DEBUG
    printf_P(PSTR("lcd::clearLine(%d)\n"), (uint8_t)line);
#endif
}

/************************************************
 * writeString()
 ************************************************
 * write a character string starting to the 
 * requested line, optionally starting later than
 * pos'n zero in the line.  posn is an optional
 * parameter with a default of zero.  Display
 * is in 'one-line' mode, meaning there will be
 * no wrap to the next line.  Any content after
 * position 20 will be truncated
 ***********************************************/
void lcd::writeString(lcd_line_t line, const char* text, uint8_t posn)
{
#ifdef DEBUG
    printf_P(PSTR("lcd::writeString(0x%02x, %d, >%s<)\n"), (uint8_t)line, posn, text);
#endif

    const char* c = text;
    size_t len = strlen(text);

    if (len + posn >= MAX_LINE_LENGTH)
    {
        len = MAX_LINE_LENGTH - posn - 1;
    }

    // set the cursor for the first position.  it should 
    // auto-increment for each char written
    this->sendCmd((uint8_t)line + posn);
    
    for (size_t ii = 0; ii < len; ii++)
    {
        this->sendChar(*c);
        ++c;
    }
}

/************************************************
 * writeChar()
 ************************************************
 * write a single character to the 
 * requested line, optionally at a later than
 * pos'n zero in the line.  posn is an optional
 * parameter with a default of zero
 ***********************************************/
void lcd::writeChar(lcd_line_t line, const char c, uint8_t posn)
{
#ifdef DEBUG
    printf_P(PSTR("LCD writeCharAt(0x%02x, %d, >%c<)\n"), (uint8_t)line, posn, c);
#endif

    if (posn >= MAX_LINE_LENGTH)
    {
        posn = MAX_LINE_LENGTH - 1;
    }

    this->sendCmd((uint8_t)line + posn);
    this->sendChar(c);
}

/************************************************
 * init()
 ************************************************
 * initialize the display.  4-bit control, 
 * display on, cursor hidden.  It will be in 
 * one-line mode, meaning display will not 
 * wrap at the end of the line
 ***********************************************/
void lcd::init()
{
    // initial I/O states
    LCD_D4_OFF();
    LCD_D5_OFF();
    LCD_D6_OFF();
    LCD_D7_OFF();
    LCD_E_ON();
    LCD_RS_OFF();
    LCD_CNTRST_OFF();

    // wait for LCD to be sure it's powered up
    _delay_ms(5);  
    _delay_ms(5);  
    _delay_ms(5);  

    // startup sequence.  The manual sez so.
    this->writeNibble(0x03);
    _delay_ms(5);
    this->writeNibble(0x03);
    _delay_ms(5);
    this->writeNibble(0x03);
    _delay_ms(5);

    // 4-bit mode
    this->writeNibble(0x02);
    _delay_ms(5);

    // display on, cursor hidden
    this->sendCmd(0x0c);

    // 4-bit mode, one-line mode (do not wrap at end of line)
    this->sendCmd(0x14);

#ifdef DEBUG
    printf_P(PSTR("LCD init done\n"));
#endif
}

/************************************************
 * writeNibble()
 ************************************************
 * write a single 4-bit value to the display's
 * data lines.
 ***********************************************/
void lcd::writeNibble(uint8_t b)
{
    cli();
    LCD_E_ON();
    
    LCD_D4_SET(GETBIT(b, 0));
    LCD_D5_SET(GETBIT(b, 1));
    LCD_D6_SET(GETBIT(b, 2));
    LCD_D7_SET(GETBIT(b, 3));

    _delay_us(10);

    LCD_E_OFF();
    _delay_us(30);
    sei();
#ifdef DEBUG
    printf_P(PSTR("lcd::writeNibble(0x%x), RS %c e %c 0x%x\n"), 
        b & 0x0f, 
        ((PORTC & 0x01) == 0x01) ? '1' : '0',
        ((PORTC & 0x02) == 0x02) ? '1' : '0',
        PORTC >> 2);
#endif
}

/************************************************
 * writeByte()
 ************************************************
 * write a single byte to the display, first 
 * sending upper nibble, and then the lower 
 * nibble
 ***********************************************/
void lcd::writeByte(uint8_t b)
{
    uint8_t high = b >> 4;
    uint8_t low = b & 0x0f;

    // send the byte, high nibble first
    this->writeNibble(high);
    this->writeNibble(low);
}

/************************************************
 * sencChar()
 ************************************************
 * send a single character to the display.  It
 * is assumed address has been properly set up
 * first
 ***********************************************/
void lcd::sendChar(char c)
{
    LCD_RS_ON();
    this->writeByte(c);
}

/************************************************
 * sendCmd()
 ************************************************
 * send a command to the display
 ***********************************************/
void lcd::sendCmd(uint8_t cmd)
{
    LCD_RS_OFF();
    this->writeByte(cmd);
}

/*****************************************************
* PWM timer, every .04ms
******************************************************
* This is a 20Khz timer, but PWM counter is 256, so
* actual PWM frequency comes out to 78-1/8 Hz
*****************************************************/
ISR(TIMER1_COMPA_vect, ISR_BLOCK) 
{
    static volatile uint8_t count = 0; 
    ++count;

    if (count < backlightPWMVal)
    {
        LCD_CNTRST_ON();
    }
    else
    {
        LCD_CNTRST_OFF();
    }
}