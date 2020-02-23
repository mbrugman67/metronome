
#include "uiBpm.h"

#include <string.h>
#include <stdio.h>

//                                 12345678901234567890
const char line2[] PROGMEM = "Beats/Min: XXX      ";
const char line3[] PROGMEM = "UP/DN to adjust     ";
const char line4[] PROGMEM = "Entr=set, Mnu=next  ";

void uiBpm::start()
{
    display->writePString(LINE_1, line1);
    display->writePString(LINE_2, line2);
    display->writePString(LINE_3, line3);
    display->writePString(LINE_4, line4);

    bpm = settings->getBPM();
    this->updateBPMLine();
    saveDelay = 0;
}

void uiBpm::update(bool& change)
{
    change = false;
    ++count;

    if (btns->menuOneShot())
    {
        change = true;
        return;
    }

    if (doUpDown(bpm, 30, 240, 60))
    {
        this->updateBPMLine();
    }

    if (saveDelay)
    {
        --saveDelay;

        if (!saveDelay)
        {
            settings->setBPM(bpm);
            settings->saveNVM();
        }
    }
}


void uiBpm::updateBPMLine()
{
    snprintf(buffer, 20, "%3d", bpm);
    display->writeString(LINE_2, buffer, 11);
}
