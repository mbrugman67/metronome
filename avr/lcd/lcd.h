/***************************************************
 * file: lcd.h
 ***************************************************
 * handle standard Hitachi-interfaced LCD.  This
 * particular unit is a 4x20 and will be used in
 * 4-bit mode
 **************************************************/
#ifndef LCD_H_
#define LCD_H_

#include <avr/pgmspace.h>

#include "../project.h"
#include "../sys/ioDefinitions.h"

extern volatile uint8_t backlightPWMVal;

// line number starting addresses, including bit 8
// (which is the command bit for 'address')
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

    void writeString(lcd_line_t line, const char* text, uint8_t posn = 0);
    void writeChar(lcd_line_t line, const char c, uint8_t posn = 0);

private:
    lcd(bool debug = false) {}
    virtual ~lcd() {}
    static lcd* _inst;
    void init();

    void writeNibble(uint8_t b);
    void writeByte(uint8_t b);
    void sendChar(char c);
    void sendCmd(uint8_t cmd);
};

#endif