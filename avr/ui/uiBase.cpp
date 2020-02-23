/***************************************************
 * file: uiBase.cpp
 ***************************************************
 * Base class for menu handler.  Each derived class
 * will be a single menu function.  In an ideal 
 * c++ world, this would be a pure virtual class,
 * but without an implementation of libstdc++ this
 * isn't possible.  (Same reason that you can't
 * use new/delete)
 **************************************************/

#include "uiBase.h"

/****************************************************
 * constructor
 ****************************************************
 * get pointers to instances of the singletons
 ***************************************************/
uiBase::uiBase()
{
    settings = nvm::getInstance();
    btns = buttons::getInstance();
    display = lcd::getInstance();
    leds = LEDString::getInstance();

    next = NULL;

    count = 0;
    saveDelay = 0;
}


/******************************************************
 * doUpDown()
*******************************************************
    * generic method to increment/decrement a value on 
    * button press.  Upper and lower bound is honored.
    * value changes rapidly when button held, at a rate
    * of about one every 16ms; it should take about 4 
    * seconds to scroll from zero to 256
*****************************************************/
bool uiBase::doUpDown(uint16_t& val, const uint16_t low, const uint16_t hi, const uint16_t dflt)
{
    bool changed = false;

    if (btns->isDownHeld() && btns->isUpHeld())
    {
        if (val != (dflt - 1))
        {
            val = dflt - 1;
            changed = true;
            saveDelay = WAIT_4_SAVE_TIME;
        }
    } 
    else if (btns->downOneShot() || (btns->isDownHeld() && !(count % 8)))
    {
        if (val > low)
        {
            --val;
            changed = true;
            saveDelay = WAIT_4_SAVE_TIME;
        }
    }
    else if (btns->upOneShot() || (btns->isUpHeld() && !(count % 8)))
    {
        if (val < hi)
        {
            ++val;
            changed = true;
            saveDelay = WAIT_4_SAVE_TIME;
        }
    }

    return (changed);
}