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
#include "../buttons/buttons.h"
#include "../lcd/lcd.h"
#include "../nvm/nvm.h"
#include "../string/LEDString.h"

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
    uint16_t getBPM()       { return (bpm); }

private:
    bool start;
    bool stop;
    bool pretty;
    uint16_t bpm;
    uint16_t saveTime;
    menu_state_t state;
    action_state_t action;

    char buffer[21];

    buttons* btns;
    lcd* display;
    nvm* settings;
    LEDString* leds;

    void stateSetBPM();
    void stateIdle();
    void stateStartPretty();

    void updateBPMLine(bool home = false);
    void updateContrastLine();
};

#endif // UI_H_