
#include "uiIdle.h"
#include <stdio.h>

//                                  12345678901234567890
static const char line2[] PROGMEM = {"XXX BPM, Sweep mode "};
static const char line3[] PROGMEM = {"Entr to start       "};
static const char line4[] PROGMEM = {"Menu for settings   "};
const char menuName[] PROGMEM = "menu idle";

PGM_P uiIdle::getName()
{
    return (menuName);
}

void uiIdle::start()
{
    display->writePString(LINE_1, line1);
    display->writePString(LINE_2, line2);
    display->writePString(LINE_3, line3);
    display->writePString(LINE_4, line4);

    snprintf(buffer, 20, "%3d", settings->getBPM());
    display->writeString(LINE_2, buffer);

    running = false;
}

void uiIdle::update(bool& change)
{
    if (btns->menuOneShot() && !running)
    {
        change = true;
        return;
    }

    if (btns->enterOneShot())
    {
        printf_P(PSTR("WTF1\n"));
        if (!running)
        {
            leds->start(settings->getBPM());
            running = true;
        }
        else
        {
            leds->stop();
            running = false;
        }
    }

}