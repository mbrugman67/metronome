
#include <stdlib.h>

#include "buttons.h"
#include "../project.h"

#ifdef DEBUG
#include <stdio.h>
#include <avr/pgmspace.h>
#endif

#define DEBOUNCE_TIME   20 / (TASK_INTERVAL)
#define HOLD_TIME       500 / (TASK_INTERVAL)

buttons* buttons::_inst = NULL;

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
#ifdef DEBUG
    static button_t last;
    if (last.menu != btns.menu)
    {
        printf_P(PSTR("Menu button from %s to %s\n"),
            this->buttonStateToText(last.menu),
            this->buttonStateToText(btns.menu));
        
        last.menu = btns.menu;
    }

    if (last.up != btns.up)
    {
        printf_P(PSTR("Up button from %s to %s\n"),
            this->buttonStateToText(last.up),
            this->buttonStateToText(btns.up));

        last.up = btns.up;
    }    

    if (last.down != btns.down)
    {
        printf_P(PSTR("Down button from %s to %s\n"),
            this->buttonStateToText(last.down),
            this->buttonStateToText(btns.down));

        last.down = btns.down;
    }    

    if (last.enter != btns.enter)
    {
        printf_P(PSTR("Enter button from %s to %s\n"),
            this->buttonStateToText(last.enter),
            this->buttonStateToText(btns.enter));

        last.enter = btns.enter;
    }    
#endif

    ++btns.menuTime;
    ++btns.upTime;
    ++btns.downTime;
    ++btns.enterTime;
    
    if (MENU_BUTTON())
    {
        btns.menuTime = 0;
        btns.menu = BUTTON_OFF;
    } 
    else if (btns.menuTime > HOLD_TIME)
    {
        btns.menuTime = HOLD_TIME + 1;
        btns.menu = BUTTON_HELD;
    }
    else if (btns.menuTime > DEBOUNCE_TIME)
    {
        btns.menu = BUTTON_DOWN;
    }
    
    if (UP_BUTTON())
    {
        btns.upTime = 0;
        btns.up = BUTTON_OFF;
    } 
    else if (btns.upTime > HOLD_TIME)
    {
        btns.upTime = HOLD_TIME + 1;
        btns.up = BUTTON_HELD;
    }
    else if (btns.upTime > DEBOUNCE_TIME)
    {
        btns.up = BUTTON_DOWN;
    }
    
    if (DOWN_BUTTON())
    {
        btns.downTime = 0;
        btns.down = BUTTON_OFF;
    } 
    else if (btns.downTime > HOLD_TIME)
    {
        btns.downTime = HOLD_TIME + 1;
        btns.down = BUTTON_HELD;
    }
    else if (btns.downTime > DEBOUNCE_TIME)
    {
        btns.down = BUTTON_DOWN;
    }
    
    if (ENTER_BUTTON())
    {
        btns.enterTime = 0;
        btns.enter = BUTTON_OFF;
    } 
    else if (btns.enterTime > HOLD_TIME)
    {
        btns.enterTime = HOLD_TIME + 1;
        btns.enter = BUTTON_HELD;
    }
    else if (btns.enterTime > DEBOUNCE_TIME)
    {
        btns.enter = BUTTON_DOWN;
    }
}
