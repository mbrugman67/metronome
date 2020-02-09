#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <stdint.h>
#include "../sys/ioDefinitions.h"

class buttons
{
public:
    static buttons* getInstance();
    void update();

    bool isMenuPressed()        { return (menu); }
    bool isMenuHeld()           { return (menuHeld); }

    bool isUpPressed()          { return (up); }
    bool isUpHeld()             { return (upHeld); }

    bool isDownPressed()        { return (down); }  
    bool isDownHeld()           { return (downHeld); }

    bool isEnterPressed()       { return (enter); }
    bool isEnterHeld()          { return (enterHeld); }

private:
    buttons() {}
    virtual ~buttons() {}
    void init();

    static bool menu;
    static bool menuHeld;
    static bool up;
    static bool upHeld;
    static bool down;
    static bool downHeld;
    static bool enter;
    static bool enterHeld;

    uint8_t menuTime;
    uint8_t upTime;
    uint8_t downTime;
    uint8_t enterTime;

    static buttons* _inst;
};

#endif // BUTTONS_H_