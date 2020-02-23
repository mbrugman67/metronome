
#include "uiIdle.h"
#include <stdio.h>

//                                  12345678901234567890
static const char line2[] PROGMEM = {"XXX BPM, Sweep mode "};
static const char line3[] PROGMEM = {"Entr to start       "};
static const char line4[] PROGMEM = {"Menu for settings   "};

void uiIdle::start()
{
    display->writePString(LINE_1, line1);
    display->writePString(LINE_2, line2);
    display->writePString(LINE_3, line3);
    display->writePString(LINE_4, line4);

    snprintf(buffer, 20, "%3d", settings->getBPM());
    display->writeString(LINE_2, buffer);
}

void uiIdle::update(bool& change)
{
    if (btns->menuOneShot())
    {
        change = true;
        return;
    }

}