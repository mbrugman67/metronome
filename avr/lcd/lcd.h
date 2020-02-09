#ifndef LCD_H_
#define LCD_H_

#include <util/delay.h>

#include "../project.h"
#include "../sys/ioDefinitions.h"

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

    char blank[20];
    char lineBuffer[21];

    void init();
    void writeByte(uint8_t b);
    void sendChar(char c);
    void sendCmd(uint8_t cmd);

    inline void toggleELine()
    {
        LCD_E_OFF();
        _delay_us(40);
        LCD_E_ON();
    }
};

#endif