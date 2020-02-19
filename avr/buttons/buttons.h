/***************************************************
 * file: buttons.h
 ***************************************************
 * handle hardware buttons.  Buttons may be in 
 * one of 3 possible states:
 *  BUTTON_OFF - button is release
 *  BUTTON_DOWN - button has been pressed
 *  BUTTON_HELD - button has been held down
 * 
 * There are four buttons:
 *  Menu
 *  Up Arrow
 *  Down Arrow
 *  Enter
 * 
 * This class handles those things, plus debounce 
 * for the "down" state
 **************************************************/
#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <stdint.h>
#include "../sys/ioDefinitions.h"

#define BUTTON_OFF      (uint8_t)0x00   // Up, or unpressed
#define BUTTON_DOWN     (uint8_t)0x01   // pressed, but not yet held
#define BUTTON_HELD     (uint8_t)0x02   // pressed and held
class buttons
{
public:
    // this is a singleton class
    static buttons* getInstance();

    // this method must be called exactly once per task time
    void update();

    // four buttons: Menu, Up, Down, Enter
    bool isMenuPressed()        { return (isButtonOn(btns[MENU].state)); }
    bool isMenuHeld()           { return (isButtonHeld(btns[MENU].state)); }
    bool menuOneShot()          { return (btns[MENU].oneShot); }

    bool isUpPressed()          { return (isButtonOn(btns[UP].state)); }
    bool isUpHeld()             { return (isButtonHeld(btns[UP].state)); }
    bool upOneShot()            { return (btns[UP].oneShot); }

    bool isDownPressed()        { return (isButtonOn(btns[DOWN].state)); }
    bool isDownHeld()           { return (isButtonHeld(btns[DOWN].state)); }
    bool downOneShot()          { return (btns[DOWN].oneShot); }

    bool isEnterPressed()       { return (isButtonOn(btns[MENU].state)); }
    bool isEnterHeld()          { return (isButtonHeld(btns[MENU].state)); }
    bool enterOneShot()         { return (btns[MENU].oneShot); }

private:
    enum BUTTONS 
    {
        MENU,
        UP,
        DOWN,
        ENTER,
        BUTTON_COUNT
    };

    buttons() {}
    virtual ~buttons() {}
    void init();

    // Handy struct to hold everything for 
    // all four buttons
    struct button_t
    {
        uint8_t     state : 2;
        uint8_t     oneShot : 1;
        uint8_t     oneShotLatch : 1;
        uint8_t     padding : 4;
        uint8_t     time;
        char        name[8];
        bool        (*btnFnc)(void);
    };

    button_t btns[BUTTON_COUNT];

    inline void setButton(uint8_t btn, uint8_t state)
    {
        btn = state;
    }

    inline bool isButtonOff(uint8_t btn)
    {
        return (btn == BUTTON_OFF);
    }

    inline bool isButtonOn(uint8_t btn)
    {
        return (btn == BUTTON_DOWN);
    }

    inline bool isButtonHeld(uint8_t btn)
    {
        return (btn == BUTTON_HELD);
    }

    static buttons* _inst;

#ifdef DEBUG
    inline const char* buttonStateToText(uint8_t s)
    {
        switch (s)
        {
            case BUTTON_OFF:     return ("up");     break;
            case BUTTON_DOWN:   return ("down");    break;
            case BUTTON_HELD:   return ("held");    break;
        }
        return ("");
    }
#endif
};

#endif // BUTTONS_H_