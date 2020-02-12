/***************************************************
 * file: lcd.h
 ***************************************************
 * handle standard Hitachi-interfaced LCD.  This
 * particular unit is a 4x20 and will be used in
 * 4-bit mode
 **************************************************/
#ifndef LCD_H_
#define LCD_H_

#include <util/delay.h>

#include "../project.h"
#include "../sys/ioDefinitions.h"

extern volatile uint8_t backlightPWMVal;

enum lcd_line_t
{
    LINE_1 = 0x80,
    LINE_2 = 0xc0,
    LINE_3 = 0x94,
    LINE_4 = 0xd4 
};
class lcd
{
public: 
    static lcd* getInstance();

    void clearAll();
    void clearLine(lcd_line_t line);

    void writeLine(lcd_line_t line, char* text);
    void writeLineAt(lcd_line_t line, uint8_t posn, char* text);
    void writeLine(lcd_line_t line, const char* text);
    void writeLineAt(lcd_line_t line, uint8_t posn, const char* text);
    void writeCharAt(lcd_line_t line, uint8_t posn, char c);

private:
    lcd(bool debug = false) {}
    virtual ~lcd() {}
    static lcd* _inst;
    void init();

    char blank[20];
    char lineBuffer[21];

    void writeNibble(uint8_t b);
    void writeByte(uint8_t b);
    void sendChar(char c);
    void sendCmd(uint8_t cmd);
};

#endif