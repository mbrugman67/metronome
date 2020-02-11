/***************************************************
 * file: ui.cpp
 ***************************************************
 * Class to handle user input and output.
 * Inputs are in the form of button presses, output
 * is to a 4x20 LCD.
 * 
 * The whole thing is menu driven; this class does
 * that and reports state out
 **************************************************/
#include "ui.h"

#ifdef DEBUG
#include <stdio.h>
#include <avr/pgmspace.h>
#endif

/****************************************
 * init()
 ****************************************
 * Set up local variables, get ready to
 * accept input
 ***************************************/
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


/****************************************
 * update()
 ****************************************
 * Should be called exactly once per
 * task interval
 ***************************************/
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