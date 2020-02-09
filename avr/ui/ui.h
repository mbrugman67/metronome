#ifndef UI_H_
#define UI_H_

#include <stdint.h>

#include "../project.h"
#include "../buttons/buttons.h"
#include "../lcd/lcd.h"

class ui
{
    enum menu_state_t
    {
        MENU_IDLE,
        MENU_SET_BPM,
        MENU_START_PATTERN
    };

    enum action_state_t
    {
        ACTION_STOPPED,
        ACTION_STARTING,
        ACTION_RUNNING,
        ACTION_STARTING_PRETTY,
        ACTION_PRETTY
    };    

public:
    ui() {}
    ~ui() {}

    void init();
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
    menu_state_t state;
    action_state_t action;

    buttons* io;
    lcd* display;

    void stateSetBPM();
    void stateIdle();
    void sateStartPretty();
};

#endif // UI_H_