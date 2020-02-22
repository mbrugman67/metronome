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
#include "flashStrings.h"

#include <stdio.h>
#include <string.h>


// wait a fixed time after the last button press before
// commiting to EEPROM
#define WAIT_4_SAVE_TIME        2000 / TASK_INTERVAL

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
    saveTime = 0;
    scrollDelay = 0;

    state = MENU_STATE_IDLE;
    action = ACTION_STOPPED;

    btns = buttons::getInstance();
    display = lcd::getInstance();
    settings = nvm::getInstance();
    leds = LEDString::getInstance();

    bpm = settings->getBPM();
    contrast = settings->getContrast();

    display->setContrast(contrast);

    // initial display
    this->writeDisplay(LINE_HOME);
    this->updateBPMLine(true);

#ifdef DEBUG
    printf_P(PSTR("UI init'd\n"));
#endif
}

void ui::stateSetContrast()
{
    this->checkAdjustContrast();

    if (btns->menuOneShot())
    {
        this->writeDisplay(LINE_HOME);
        this->updateBPMLine(true);

        state = MENU_STATE_IDLE;
    }
}

void ui::stateSetBPM()
{
    this->checkAdjustBPM();

    if (btns->menuOneShot())
    {
        this->writeDisplay(LINE_CNT);
        this->updateContrastLine();
        state = MENU_STATE_CONTRAST;
    }
}

void ui::stateIdle()
{
    if (btns->menuOneShot())
    {
        this->writeDisplay(LINE_BPM);
        this->updateBPMLine();
        state = MENU_STATE_BPM;
    }
}

/****************************************
 * checkAdjustBPM()
 ****************************************
 * Update BPM setting.  Increase/Decrease
 * by one on the press, then scroll one
 * beat every 16ms when held.  It would
 * take 3 seconds to scroll through the
 * entire range of 30 to 240 BPM
 ***************************************/
void ui::checkAdjustBPM()
{
    // if both up and down are held, jump BPM 
    // to default value
    if (btns->isUpHeld() && btns->isDownHeld())
    {
        if (bpm != DEFAULT_BPM - 1)
        {
            bpm = DEFAULT_BPM - 1;
            saveTime = WAIT_4_SAVE_TIME;
        }
    }
    if (btns->upOneShot() || (btns->isUpHeld() && !(scrollDelay % 8)))
    {
        if (bpm < 240)
        {
            ++bpm;
            updateBPMLine();
            saveTime = WAIT_4_SAVE_TIME;
        }
    }
    else if (btns->downOneShot() || (btns->isDownHeld() && !(scrollDelay % 8)))
    {
        if (bpm > 20)
        {
            --bpm;
            updateBPMLine();
            saveTime = WAIT_4_SAVE_TIME;
        }
    }
}

/****************************************
 * checkAdjustContrast()
 ****************************************
 * Update BPM setting.  Increase/Decrease
 * by one on the press, then scroll one
 * beat every 16ms when held.  It would
 * take 3 seconds to scroll through the
 * entire range of 30 to 240 BPM
 ***************************************/
void ui::checkAdjustContrast()
{
    // if both up and down are held, jump BPM 
    // to default value
    if (btns->isUpHeld() && btns->isDownHeld())
    {
        if (contrast != DEFAULT_CONTRAST - 1)
        {
            contrast = DEFAULT_CONTRAST - 1;
            this->updateContrastLine();
            display->setContrast(contrast);
            saveTime = WAIT_4_SAVE_TIME;
        }
    }
    if (btns->upOneShot() || (btns->isUpHeld() && !(scrollDelay % 8)))
    {
        if (contrast < 100)
        {
            ++contrast;
            this->updateContrastLine();
            display->setContrast(contrast);
            saveTime = WAIT_4_SAVE_TIME;
        }
    }
    else if (btns->downOneShot() || (btns->isDownHeld() && !(scrollDelay % 8)))
    {
        if (contrast > 0)
        {
            --contrast;
            updateContrastLine();
            display->setContrast(contrast);
            saveTime = WAIT_4_SAVE_TIME;
        }
    }
}


void ui::stateStartPretty()
{

}

void ui::updateBPMLine(bool home)
{
    uint8_t posn = 0;
    if (!home)
    {
        posn = 11;
    }

    snprintf(buffer, 20, "%3d", bpm);
    display->writeString(LINE_2, buffer, posn);
}

void ui::updateContrastLine()
{
    snprintf(buffer, 20, "%3d", display->getContrast());
    display->writeString(LINE_2, buffer, 9);
}

void ui::writeDisplay(uint8_t scrnInx)
{
    display->writePString(LINE_1, getPStrPtr(line1[scrnInx]));
    display->writePString(LINE_2, getPStrPtr(line2[scrnInx]));
    display->writePString(LINE_3, getPStrPtr(line3[scrnInx]));
    display->writePString(LINE_4, getPStrPtr(line4[scrnInx]));
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
    static menu_state_t lastState = MENU_STATE_PATTERN;
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
        case MENU_STATE_IDLE:
        {
            this->stateIdle();
        }  break;

        case MENU_STATE_BPM:
        {
            this->stateSetBPM();
        }  break;

        case MENU_STATE_PATTERN:
        {

        }  break;

        case MENU_STATE_CONTRAST:
        {
            this->stateSetContrast();
        }  break;

        case MENU_STATE_INFO:
        {

        }  break;
    }

    // used to prevent super fast scrolling
    ++scrollDelay;

    // check to see if EEPROM right is pending
    if (saveTime)
    {
        --saveTime;
        if (!saveTime)
        {
            settings->setBPM(bpm);
            settings->setContrast(contrast);
            settings->saveNVM();
            printf_P(PSTR("Saved new BPM of %d\n"), bpm);
        }
    }
}