
#include <stdlib.h>

#include "buttons.h"
#include "../project.h"

#define DEBOUNCE_TIME   20 / (TASK_INTERVAL)
#define HOLD_TIME       500 / (TASK_INTERVAL)

buttons* buttons::_inst = NULL;
bool buttons::up = false;
bool buttons::upHeld = false;
bool buttons::menu = false;
bool buttons::menuHeld = false;
bool buttons::down = false;
bool buttons::downHeld = false;
bool buttons::enter = false;
bool buttons::enterHeld = false;

buttons* buttons::getInstance()
{
    if (!_inst)
    {
        _inst = (buttons*)malloc(sizeof(buttons));
    }

    return (_inst);
}

void buttons::update()
{
    ++menuTime;
    ++upTime;
    ++downTime;
    ++enterTime;
    
    if (!MENU_BUTTON())
    {
        menuTime = 0;
        menu = false;
        menuHeld = false;
    } 
    else if (menuTime > HOLD_TIME)
    {
        menuTime = HOLD_TIME + 1;
        menu = false;
        menuHeld = true;
    }
    else if (menuTime > DEBOUNCE_TIME)
    {
        menu = true;
    }
    
    if (!UP_BUTTON())
    {
        upTime = 0;
        up = false;
        upHeld = false;
    } 
    else if (upTime > HOLD_TIME)
    {
        upTime = HOLD_TIME + 1;
        up = false;
        upHeld = true;
    }
    else if (upTime > DEBOUNCE_TIME)
    {
        up = true;
    }
    
    if (!DOWN_BUTTON())
    {
        downTime = 0;
        down = false;
        downHeld = false;
    } 
    else if (downTime > HOLD_TIME)
    {
        downTime = HOLD_TIME + 1;
        down = false;
        downHeld = true;
    }
    else if (downTime > DEBOUNCE_TIME)
    {
        down = true;
    }
    
    if (!MENU_BUTTON())
    {
        menuTime = 0;
        menu = false;
        menuHeld = false;
    } 
    else if (menuTime > HOLD_TIME)
    {
        menuTime = HOLD_TIME + 1;
        menu = false;
        menuHeld = true;
    }
    else if (menuTime > DEBOUNCE_TIME)
    {
        menu = true;
    }
}
