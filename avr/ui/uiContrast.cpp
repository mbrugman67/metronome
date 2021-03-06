
#include "uiContrast.h"

#include <string.h>
#include <stdio.h>

//                                 12345678901234567890
const char line2[] PROGMEM = {"Contrast:           "};
const char line3[] PROGMEM = {"UP/DN to adjust     "};
const char line4[] PROGMEM = {"Entr=set, Mnu=next  "};
const char menuName[] PROGMEM = "menu set contrast";

PGM_P uiContrast::getName()
{
    return (menuName);
}

void uiContrast::start()
{
    display.writePString(LINE_1, line1);
    display.writePString(LINE_2, line2);
    display.writePString(LINE_3, line3);
    display.writePString(LINE_4, line4);

    contrast = settings.getContrast();
    this->updateContrastLine();
    saveDelay = 0;
}

void uiContrast::update(bool& change)
{
    change = false;
    ++count;

    if (btns.menuOneShot())
    {
        change = true;
        return;
    }

    if (doUpDown(contrast, 0, 100, DEFAULT_CONTRAST))
    {
        this->updateContrastLine();
        display.setContrast(contrast);
    }

    if (btns.enterOneShot() && saveDelay)
    {
        saveDelay = 0;

        settings.setContrast(contrast);
        settings.saveNVM();
    }
}

void uiContrast::updateContrastLine()
{
    snprintf(buffer, 20, "%3d", contrast);
    display.writeString(LINE_2, buffer, 11);
}
