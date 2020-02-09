#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <avr/pgmspace.h>

#include "lcd.h"
#include "../sys/ioDefinitions.h"

#define MAX_LINE_LENGTH     20
#define MAX_LINE_COUNT      4

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
    _delay_ms(1.64);     // This command takes 1.64 ms

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

#ifdef DEBUG
    printf_P(PSTR("LCD writeLineAt(%d, %d, >%s<)\n"), (uint8_t)line, posn, text);
#endif
}

void lcd::writeCharAt(lcd_line_t line, uint8_t posn, char c)
{
    if (posn > MAX_LINE_LENGTH)
    {
        posn = MAX_LINE_LENGTH;
    }

    this->sendCmd((uint8_t)line + posn);
    this->sendChar(c);

#ifdef DEBUG
    printf_P(PSTR("LCD writeCharAt(%d, %d, >%c<)\n"), (uint8_t)line, posn, c);
#endif
}

void lcd::init()
{
    LCD_E_ON();
    LCD_RS_OFF();

    // wait for LCD to be sure it's powered up
    _delay_ms(15.0);   

    for (size_t ii = 0; ii < 3; ++ii)
    {
        this->writeByte(0x03);
        _delay_ms(1.64);
    }

    this->writeByte(0x02);     // 4-bit mode
    _delay_us(40.0);    // most instructions take 40us
 
    this->sendChar(0x28); // Function set 001 BW N F - -
    this->sendChar(0x0C);
    this->sendChar(0x6); 

    strncpy(blank, "                    ", 20);

#ifdef DEBUG
    printf_P(PSTR("LCD init done\n"));
#endif
}

void lcd::writeByte(uint8_t b)
{
    // high nibble first
    LCD_D4_SET(GETBIT(b, 4));
    LCD_D5_SET(GETBIT(b, 5));
    LCD_D6_SET(GETBIT(b, 6));
    LCD_D7_SET(GETBIT(b, 7));
    _delay_us(40.0);

    this->toggleELine();

    // high nibble first
    LCD_D4_SET(GETBIT(b, 0));
    LCD_D5_SET(GETBIT(b, 1));
    LCD_D6_SET(GETBIT(b, 2));
    LCD_D7_SET(GETBIT(b, 3));
    _delay_us(40.0);

    this->toggleELine();
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
