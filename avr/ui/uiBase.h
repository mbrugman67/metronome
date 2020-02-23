/***************************************************
 * file: uiBase.h
 ***************************************************
 * Base class for menu handler.  Each derived class
 * will be a single menu function.  In an ideal 
 * c++ world, this would be a pure virtual class,
 * but without an implementation of libstdc++ this
 * isn't possible.  (Same reason that you can't
 * use new/delete)
 **************************************************/

#ifndef UI_BASE_H_
#define UI_BASE_H_

#include "../project.h"
#include "../nvm/nvm.h"
#include "../string/LEDString.h"
#include "../buttons/buttons.h"
#include "../lcd/lcd.h"

#include <stdlib.h>
#include <stdint.h>

#include <avr/pgmspace.h>

// wait a fixed time after the last button press before
// commiting to EEPROM
#define WAIT_4_SAVE_TIME        2000 / TASK_INTERVAL

static const char line1[] PROGMEM = "MNU   UP   DN   ENTR";

class uiBase
{
public:
    uiBase();
    ~uiBase() {}

    // Ideally, these would be pure virtual methods; in fact this should
    // probably be a pure virtual class.  However, avr-gcc doesn't have
    // an implementation of libstdc++, which makes some c++ patterns
    // impossible.  :/
    virtual void update(bool& change) {}
    virtual void start() {}
    virtual PGM_P getName() { return NULL; }

    // the menus are driven as a singly-linked list
    uiBase* getNextState()          { return(next); }
    void setNextState(uiBase* ns)   { next = ns; }


protected:
    char buffer[21];
    uint8_t count;
    uint16_t saveDelay;

    uiBase* next;
    nvm* settings;
    buttons* btns;
    lcd* display;
    LEDString* leds;

    bool doUpDown(uint16_t& val, const uint16_t low, const uint16_t hi, const uint16_t dflt);

private:

};

#endif // UI_BASE_H_