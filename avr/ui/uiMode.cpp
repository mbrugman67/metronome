/***************************************************
 * file: uiMode.h
 ***************************************************
 * Derived menu class
 * This will adjust the metronome mode
 **************************************************/

#include "uiMode.h"

#include <string.h>
#include <stdio.h>

//                            12345678901234567890
const char line2[] PROGMEM = "Mode:               ";
const char line3[] PROGMEM = "UP to change        ";
const char line4[] PROGMEM = "Entr=set, Mnu=next  ";
const char menuName[] PROGMEM = "menu set mode";

PGM_P uiMode::getName()
{
    return (menuName);
}

void uiMode::start()
{
    display.writePString(LINE_1, line1);
    display.writePString(LINE_2, line2);
    display.writePString(LINE_3, line3);
    display.writePString(LINE_4, line4);

    mode = settings.getMode();
    this->updateModeLine(mode);
    
    saveDelay = 0;
}

void uiMode::updateModeLine(uint16_t m)
{
    printf_P(PSTR("updateModeLine %d\n"), m);
    switch (m)
    {
        case LEDString::MODE_METRO:     display.writePString(LINE_2, PSTR("Metro"), 6); break;
        case LEDString::MODE_PULSE:     display.writePString(LINE_2, PSTR("Pulse"), 6); break;
        case LEDString::MODE_WHQ:       display.writePString(LINE_2, PSTR(" WHQ "), 6); break; 
    }
}

void uiMode::update(bool& change)
{
    change = false;
    ++count;

    if (btns.menuOneShot())
    {
        change = true;
        return;
    }

    if (btns.upOneShot())
    {
        mode += 1;
        mode %= 3;
        this->updateModeLine(mode);

        saveDelay = WAIT_4_SAVE_TIME;
    }

    if (btns.enterOneShot() && saveDelay)
    {
        saveDelay = 0;

        settings.setMode(mode);
        settings.saveNVM();
    }
}
