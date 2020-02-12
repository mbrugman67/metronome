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

#include "lcd.h"
#include "../sys/ioDefinitions.h"

volatile uint8_t backlightPWMVal;

#ifdef DEBUG
#undef DEBUG
#endif

#ifdef DEBUG
#include <stdio.h>
#include <avr/pgmspace.h>
#endif

#define MAX_LINE_LENGTH     20

lcd* lcd::_inst = NULL;

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

void lcd::clearAll()
{
    this->sendCmd(0x01); // clear the whole thang
    _delay_ms(2);     // This command takes 1.64 ms

#ifdef DEBUG
    printf_P(PSTR("LCD total clear done\n"));
#endif
}

void lcd::clearLine(lcd_line_t line)
{
    this->sendCmd((uint8_t)line);

    for (size_t ii = 0; ii < 10; ++ii)
    {
        this->sendChar(0x20);
    }

#ifdef DEBUG
    printf_P(PSTR("LCD Clear line %d\n"), (uint8_t)line);
#endif
}

void lcd::writeLine(lcd_line_t line, char* text)
{
    this->writeLineAt(line, 0, text);
}

void lcd::writeLine(lcd_line_t line, const char* text)
{
    for (size_t posn = 0; posn < strlen(text); posn++)
    {
        this->sendCmd((uint8_t)line + posn);
        this->sendChar(text[posn]);
    }
}

void lcd::writeLineAt(lcd_line_t line, uint8_t posn, const char* text)
{
    strncpy(lineBuffer, text, 20);
    this->writeLineAt(line, posn, lineBuffer);
}

void lcd::writeLineAt(lcd_line_t line, uint8_t posn, char* text)
{
#ifdef DEBUG
    printf_P(PSTR("LCD writeLineAt(0x%02x, %d, >%s<)\n"), (uint8_t)line, posn, text);
#endif

    char* c = text;
    size_t len = strlen(text);

    if (len + posn > MAX_LINE_LENGTH)
    {
        len = MAX_LINE_LENGTH - posn;
    }

    // locate cursor at beginning of line
    this->sendCmd((uint8_t)line + posn);

    while(len)
    {
        this->sendChar(*c);
        ++c;
        --len;
    }
}

void lcd::writeCharAt(lcd_line_t line, uint8_t posn, char c)
{
#ifdef DEBUG
    printf_P(PSTR("LCD writeCharAt(0x%02x, %d, >%c<)\n"), (uint8_t)line, posn, c);
#endif

    if (posn > MAX_LINE_LENGTH)
    {
        posn = MAX_LINE_LENGTH;
    }

    this->sendCmd((uint8_t)line + posn);
    this->sendChar(c);
}

void lcd::init()
{
    LCD_D4_OFF();
    LCD_D5_OFF();
    LCD_D6_OFF();
    LCD_D7_OFF();
    LCD_E_ON();
    LCD_RS_OFF();

    // wait for LCD to be sure it's powered up
    _delay_ms(5);  
    _delay_ms(5);  
    _delay_ms(5);  

    this->writeNibble(0x03);
    _delay_ms(5);
    this->writeNibble(0x03);    //200
    _delay_ms(5);
    this->writeNibble(0x03);    // 40
    _delay_ms(5);

    this->writeNibble(0x02);     // 4-bit mode
    _delay_ms(5);

    this->sendCmd(0x28); // Function set 001 BW N F - -
    this->sendCmd(0x0C);
    this->sendCmd(0x14);

    strncpy(blank, "                    ", 20);

#ifdef DEBUG
    printf_P(PSTR("LCD init done\n"));
#endif
}

void lcd::writeNibble(uint8_t b)
{
    cli();
    LCD_E_ON();
    
    LCD_D4_SET(GETBIT(b, 0));
    LCD_D5_SET(GETBIT(b, 1));
    LCD_D6_SET(GETBIT(b, 2));
    LCD_D7_SET(GETBIT(b, 3));

    _delay_us(20);

    LCD_E_OFF();
    _delay_us(50);
    sei();
#ifdef DEBUG
    printf_P(PSTR("lcd::writeNibble(0x%x), RS %c e %c 0x%x\n"), 
        b & 0x0f, 
        ((PORTC & 0x01) == 0x01) ? '1' : '0',
        ((PORTC & 0x02) == 0x02) ? '1' : '0',
        PORTC >> 2);
#endif
}

void lcd::writeByte(uint8_t b)
{
    uint8_t high = b >> 4;
    uint8_t low = b & 0x0f;

    // send the byte, high nibble first
    this->writeNibble(high);
    this->writeNibble(low);
}

void lcd::sendChar(char c)
{
    LCD_RS_ON();
    this->writeByte(c);
}

void lcd::sendCmd(uint8_t cmd)
{
    LCD_RS_OFF();
    this->writeByte(cmd);
}

/*****************************************************
* PWM timer, every .04ms
*****************************************************/
ISR(TIMER1_COMPA_vect, ISR_BLOCK) 
{
    static volatile uint8_t count = 0; 
    ++count;

    if (count < backlightPWMVal)
    {
        PIN_IO3_ON();
    }
    else
    {
        PIN_IO3_OFF();
    }
    
}