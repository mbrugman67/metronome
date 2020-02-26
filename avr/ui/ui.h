/***************************************************
 * file: ui.h
 ***************************************************
 * Class to handle user input and output.
 * Inputs are in the form of button presses, output
 * is to a 4x20 LCD.
 * 
 * The whole thing is menu driven; this class does
 * that and reports state out
 **************************************************/
#ifndef UI_H_
#define UI_H_

#include <stdint.h>

#include "../project.h"
#include "../lcd/lcd.h"
#include "../nvm/nvm.h"

#include "uiBase.h"
#include "uiIdle.h"
#include "uiBpm.h"
#include "uiMode.h"
#include "uiContrast.h"
#include "uiColor.h"
#include "uiInfo.h"

class ui
{
    enum menu_state_t
    {
        MENU_STATE_IDLE,
        MENU_STATE_BPM,
        MENU_STATE_PATTERN,
        MENU_STATE_CONTRAST,
        MENU_STATE_INFO
    };

    enum action_state_t
    {
        ACTION_STOPPED,
        ACTION_STARTING,
        ACTION_RUNNING
    };    

public:
    ui();
    ~ui() {}

    void update();

    bool startMetronome()   { return (start); }
    bool startPretty()      { return (pretty); }
    bool stopAll()          { return (stop); }

private:
    bool start;
    bool stop;
    bool pretty;
    menu_state_t state;
    action_state_t action;

    uiBase* currentState;
    uiIdle stateIdle;
    uiBpm stateBPM;
    uiMode stateMode;
    uiContrast stateContrast;
    uiColor stateColor;
    uiInfo stateInfo;
};

#endif // UI_H_