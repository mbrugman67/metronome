
#include "uiIdle.h"
#include <stdio.h>

//                                    12345678901234567890
static const char line2[] PROGMEM = {"XXX BPM,            "};
static const char line3[] PROGMEM = {"Entr to start       "};
static const char line4[] PROGMEM = {"Menu for settings   "};
const char menuName[] PROGMEM = "menu idle";

PGM_P uiIdle::getName()
{
    return (menuName);
}

void uiIdle::start()
{
    display.writePString(LINE_1, line1);
    display.writePString(LINE_2, line2);
    display.writePString(LINE_3, line3);
    display.writePString(LINE_4, line4);

    snprintf(buffer, 20, "%3d", settings.getBPM());
    display.writeString(LINE_2, buffer);

    switch (settings.getMode())
    {
        case LEDString::MODE_METRO:    display.writePString(LINE_2, PSTR("Metro mode"), 9); break;
        case LEDString::MODE_PULSE:    display.writePString(LINE_2, PSTR("Pulse mode"), 9); break;
        case LEDString::MODE_WHQ:      display.writePString(LINE_2, PSTR(" WHQ  mode"), 9); break;
    }

    running = false;
}

void uiIdle::update(bool& change)
{
    if (btns.menuOneShot() && !running)
    {
        change = true;
        return;
    }

    if (btns.enterOneShot())
    {
        if (!running)
        {
            ledstring.start((LEDString::led_mode_t)settings.getMode(), settings.getBPM());
            running = true;
        }
        else
        {
            ledstring.stop();
            running = false;
        }
    }

}