#include "ui.h"

#ifdef DEBUG
#include <stdio.h>
#include <avr/pgmspace.h>
#endif

void ui::init()
{
    bpm = 60;
    start = false;
    pretty = false;
    stop = false;
    state = MENU_IDLE;
    action = ACTION_STOPPED;

    io = buttons::getInstance();
    display = lcd::getInstance();

#ifdef DEBUG
    printf_P(PSTR("UI init'd\n"));
#endif
}

void ui::stateSetBPM()
{

}

void ui::stateIdle()
{

}

void ui::sateStartPretty()
{

}


void ui::update()
{
#ifdef DEBUG
    static menu_state_t lastState = MENU_START_PATTERN;
    if (lastState != state)
    {
        printf_P(PSTR("Menu state change from %d to %d\n"), lastState, state);
        lastState = state;
    }
#endif

    stop = false;
    start = false;
    pretty = false;

    switch (state)
    {
        case MENU_IDLE:
        {

        }  break;

        case MENU_SET_BPM:
        {

        }  break;

        case MENU_START_PATTERN:
        {

        }  break;

    }
}