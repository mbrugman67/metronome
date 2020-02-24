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
    lcd() {};
    ~lcd() {}

    void init();

    void clearAll() const;
    void clearLine(lcd_line_t line) const;

    void writeString(lcd_line_t line, const char* text, uint8_t posn = 0) const;
    void writePString(lcd_line_t line, PGM_P text, uint8_t posn = 0) const;
    void writeChar(lcd_line_t line, const char c, uint8_t posn = 0) const;
    
    void setContrast(uint16_t contrast);
    uint16_t getContrast() const;

private:

    void writeNibble(uint8_t b) const;
    void writeByte(uint8_t b) const;
    void sendChar(char c) const;
    void sendCmd(uint8_t cmd) const;
};

#endif