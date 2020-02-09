
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
}
