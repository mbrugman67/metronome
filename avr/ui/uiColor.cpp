/***************************************************
 * file: uiColor.h
 ***************************************************
 * Derived menu class
 * This will adjust the color settings
 **************************************************/

#include "uiColor.h"

#include <string.h>
#include <stdio.h>

//                           12345678901234567890
const char line3[] PROGMEM = "UP/DN to adjust     ";
const char line4[] PROGMEM = "Entr=set, Mnu=next  ";
const char menuName[] PROGMEM = "menu set Color";

PGM_P uiColor::getName()
{
    return (menuName);
}

void uiColor::start()
{
    display.writePString(LINE_1, line1);
    display.writePString(LINE_3, line3);
    display.writePString(LINE_4, line4);

    memcpy((void*)&clr, (void*)settings.getSweepClr(), sizeof(ledclr));
    
    cstate = RED;
    this->updateColorLine();
    
    thisColor = (uint16_t)clr.getRed();
    cstate = RED;

    ledstring.startSample();

    saveDelay = 0;
}

void uiColor::updateColorLine()
{
    snprintf(buffer, 20, "%s %3d %3d %3d ",
        (cstate == RED)   ? "Rgb" :
        (cstate == GREEN) ? "rGb" : "rgB",
        clr.getRed(), clr.getGrn(), clr.getBlu());
    
    display.writeString(LINE_2, buffer);
}

void uiColor::update(bool& change)
{
    change = false;
    ++count;

    if (btns.menuOneShot())
    {
        switch (cstate)
        {
            case RED:   cstate = GREEN;  thisColor = clr.getGrn();   break;
            case GREEN: cstate = BLUE;   thisColor = clr.getBlu();   break;
            case DONE:
            case BLUE:  cstate = DONE;     break;
        }
    
        this->updateColorLine();
        
        if (cstate == DONE)
        {
            ledstring.stop();
            change = true;
            return;
        }
    }

    if (doUpDown(thisColor, 0, 255, 255))
    {
        switch (cstate)
        {
            case RED:   clr.setRed((uint8_t) thisColor);  break;
            case GREEN: clr.setGrn((uint8_t) thisColor);  break;
            case BLUE:  clr.setBlu((uint8_t) thisColor);  break;
            case DONE:  break;
        }

        this->updateColorLine();
    }

    if (btns.enterOneShot() && saveDelay)
    {
        saveDelay = 0;
        settings.setSweepColor(clr);
        settings.saveNVM();
    }
}
