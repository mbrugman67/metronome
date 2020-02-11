/***************************************************
 * file: buttons.h
 ***************************************************
 * handle hardware buttons.  Buttons may be in 
 * one of 3 possible states:
 *  BUTTON_OFF - button is release
 *  BUTTON_DOWN - button has been pressed
 *  BUTTON_HELD - button has been held down
 * 
 * This class handles those things, plus debounce 
 * for the "down" state
 **************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include "lcd.h"
#include "../sys/ioDefinitions.h"

#define MAX_LINE_LENGTH     20

lcd* lcd::_inst = NULL;

lcd* lcd::getInstance()
{
    if (!_inst)
    {
        _inst = (lcd*)malloc(sizeof(lcd));
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
    this->writeLine(line, blank);

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
    strncpy(lineBuffer, text, 20);
    this->writeLineAt(line, 0, lineBuffer);
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
    LCD_E_ON();
    LCD_RS_OFF();

    // wait for LCD to be sure it's powered up
    _delay_ms(15);  
    wdt_reset();

    this->writeNibble(0x03);
    _delay_ms(10);
    this->writeNibble(0x03);
    _delay_us(200);
    this->writeNibble(0x03);
    _delay_us(80);

    this->writeNibble(0x02);     // 4-bit mode
    _delay_us(80);    // most instructions take 40us
 
    this->sendCmd(0x28); // Function set 001 BW N F - -
    this->sendCmd(0x0C);
    this->sendCmd(0x14);
    this->sendCmd(0x01); 

    this->sendCmd(0x08);

    strncpy(blank, "                    ", 20);

#ifdef DEBUG
    printf_P(PSTR("LCD init done\n"));
#endif
}

void lcd::writeNibble(uint8_t b)
{
    DDRC = 0xff;
    LCD_D4_SET(GETBIT(b, 0));
    LCD_D5_SET(GETBIT(b, 1));
    LCD_D6_SET(GETBIT(b, 2));
    LCD_D7_SET(GETBIT(b, 3));
    _delay_us(5);

    this->toggleELine();

    _delay_us(40);

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
