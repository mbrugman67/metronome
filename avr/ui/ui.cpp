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

#include <avr/pgmspace.h>

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

    display.setContrast(settings.getContrast());

    stateIdle.setNextState(&stateBPM);
    stateBPM.setNextState(&stateMode);
    stateMode.setNextState(&stateContrast);
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
   bool changeState;
 
   currentState->update(changeState);
   if (changeState)
   {
#ifdef DEBUG
    printf_P(PSTR("State change from "));
    printf_P(currentState->getName());
#endif

       currentState = currentState->getNextState();
       currentState->start();
#ifdef DEBUG
    printf_P(PSTR(" to "));
    printf_P(currentState->getName());
    printf_P(PSTR("\n"));
#endif
   }

}