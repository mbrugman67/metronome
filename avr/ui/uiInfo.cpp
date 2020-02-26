/***************************************************
 * file: uiInfo.h
 ***************************************************
 * Derived menu class
 * This will display info
 **************************************************/

#include "uiInfo.h"

#include <string.h>
#include <stdio.h>

//                           12345678901234567890
const char line2[] PROGMEM = "Version 0.9b        ";
const char line4[] PROGMEM = "Mnu=next            ";
const char menuName[] PROGMEM = "menu get info";

PGM_P uiInfo::getName()
{
    return (menuName);
}

void uiInfo::start()
{
    display.writePString(LINE_1, line1);
    display.writePString(LINE_2, line2);
    display.writePString(LINE_4, line4);

    snprintf(buffer, 20, "Boot count: %5d  ", settings.getBootCount());
    display.writeString(LINE_3, buffer);
}

void uiInfo::update(bool& change)
{
    change = false;

    if (btns.menuOneShot())
    {
        change = true;
        return;
    }
}
