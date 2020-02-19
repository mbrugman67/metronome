/***************************************************
 * file: lcd.cpp
 ***************************************************
 * handle standard Hitachi-interfaced LCD.  This
 * particular unit is a 4x20 and will be used in
 * 4-bit mode
 * 
 * quick note:
 * _delay_us() max is 768 / F_CPU [MHz] = 48 @ 16MHz
 * _delay_ms() max is 262 / F_CPU [MHz] = 17 @ 16MHz
 **************************************************/

#include <stdlib.h>
#include <string.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lcd.h"
#include "../sys/ioDefinitions.h"

static volatile uint8_t backlightPWMVal;

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
        backlightPWMVal = 40;
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
void lcd::clearAll() const
{
    this->clearLine(LINE_1);
    this->clearLine(LINE_2);
    this->clearLine(LINE_3);
    this->clearLine(LINE_4);
    this->writeChar(LINE_1, 0x20, 0);

    // I have a somewhat brain-dead display; the clear
    // command isn't working
    //this->sendCmd(0x01); // clear the whole thang
    //_delay_ms(2);     // This command takes 1.64 ms

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
void lcd::clearLine(lcd_line_t line) const
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
void lcd::writeString(lcd_line_t line, const char* text, uint8_t posn) const
{
#ifdef DEBUG
    printf_P(PSTR("lcd::writeString(0x%02x, %d, >%s<)\n"), (uint8_t)line, posn, text);
#endif

    const char* c = text;
    size_t len = strlen(text);

    if (len + posn >= MAX_LINE_LENGTH)
    {
        len = MAX_LINE_LENGTH - posn;
    }

    this->sendCmd((uint8_t)line + posn);
    
    for (size_t ii = 0; ii < len; ii++)
    {
        //this->sendCmd((uint8_t)line + ii + posn);
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
void lcd::writeChar(lcd_line_t line, const char c, uint8_t posn) const 
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
    
void lcd::setContrast(uint16_t contrast)
{
    backlightPWMVal = (uint8_t)contrast;
}

uint16_t lcd::getContrast() const
{
    return ((uint16_t)backlightPWMVal);
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
    LCD_RS_OFF();

    // wait for LCD to be sure it's powered up
    for (size_t ii = 0; ii < 10; ++ii)
    {
        _delay_ms(5);  
    }

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
    this->sendCmd(0x28);
    _delay_ms(1);

    this->sendCmd(0x0c);
    this->sendCmd(0x06);

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
void lcd::writeNibble(uint8_t b) const
{
    LCD_E_ON();
    
    LCD_D4_SET(GETBIT(b, 0));
    LCD_D5_SET(GETBIT(b, 1));
    LCD_D6_SET(GETBIT(b, 2));
    LCD_D7_SET(GETBIT(b, 3));

    _delay_us(2);

    LCD_E_OFF();
    _delay_us(44);

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
void lcd::writeByte(uint8_t b) const
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
void lcd::sendChar(char c) const
{
    LCD_RS_ON();
    this->writeByte(c);
    LCD_RS_OFF();
}

/************************************************
 * sendCmd()
 ************************************************
 * send a command to the display
 ***********************************************/
void lcd::sendCmd(uint8_t cmd) const
{
    LCD_RS_OFF();
    this->writeByte(cmd);
}

/*****************************************************
* PWM timer, every .04ms
******************************************************
* This is a 25Khz timer, but PWM counter is 256, so
* actual PWM frequency comes out to 97.7 Hz
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