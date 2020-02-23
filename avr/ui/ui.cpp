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
#include "../project.h"
#include "ui.h"

#include <stdio.h>
#include <string.h>

/****************************************
 * constructor()
 ****************************************
 * Set up local variables, get ready to
 * accept input
 ***************************************/
ui::ui()
{
    start = false;
    pretty = false;
    stop = false;

    state = MENU_STATE_IDLE;
    action = ACTION_STOPPED;

    display = lcd::getInstance();
    settings = nvm::getInstance();

    display->setContrast(settings->getContrast());

    stateIdle.setNextState(&stateBPM);
    stateBPM.setNextState(&stateContrast);
    stateContrast.setNextState(&stateIdle);

    currentState = &stateIdle;
    currentState->start();


#ifdef DEBUG
    printf_P(PSTR("UI init'd\n"));
#endif
}

/****************************************
 * update()
 ****************************************
 * Should be called exactly once per
 * task interval
 ***************************************/
void ui::update()
{
    /*
#ifdef DEBUG
    static menu_state_t lastState = MENU_STATE_PATTERN;
    if (lastState != state)
    {
        printf_P(PSTR("Menu state change from %d to %d\n"), lastState, state);
        lastState = state;
    }
#endif
    */
   bool changeState;
   currentState->update(changeState);
   if (changeState)
   {
       currentState = currentState->getNextState();
       currentState->start();
   }

}