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

#include <stdio.h>
#include <string.h>

#include "../project.h"
#include <avr/pgmspace.h>

// wait a fixed time after the last button press before
// commiting to EEPROM
#define WAIT_4_SAVE_TIME        2000 / TASK_INTERVAL

/****************************************
 * Menu Strings
 ****************************************
 * These are stored in flash instead of
 * RAM
 ***************************************/
const char line1_comn[] PROGMEM = {"MNU   UP   DN   ENTR"};
//                                  12345678901234567890
const char line2_idle[] PROGMEM = {"XXX BPM, Sweep mode "};
const char line3_idle[] PROGMEM = {"Entr to start       "};
const char line4_idle[] PROGMEM = {"Menu for settings   "};

//                                 12345678901234567890
const char line2_bpm[] PROGMEM = {"Beats/Min: XXX      "};
const char line3_bpm[] PROGMEM = {"UP/DN to adjust     "};
const char line4_bpm[] PROGMEM = {"Entr=set, Mnu=next  "};

//                                 12345678901234567890
const char line2_cnt[] PROGMEM = {"Contrast: XXX       "};
const char line3_cnt[] PROGMEM = {"UP/DN to adjust     "};
const char line4_cnt[] PROGMEM = {"Entr=set, Mnu=next  "};

//                                  12345678901234567890
const char line2_info[] PROGMEM = {"Version:            "};
const char line3_info[] PROGMEM = {"Power Cycles: XXXXX "};
const char line4_info[] PROGMEM = {"Mnu=next            "};

#define LINE_HOME   0
#define LINE_BPM    1
#define LINE_CNT    2
#define LINE_INFO   3

PGM_P const line1[] PROGMEM = {line1_comn, line1_comn, line1_comn, line1_comn};
PGM_P const line2[] PROGMEM = {line2_idle, line2_bpm, line2_cnt, line2_info};
PGM_P const line3[] PROGMEM = {line3_idle, line3_bpm, line3_cnt, line3_info};
PGM_P const line4[] PROGMEM = {line4_idle, line4_bpm, line4_cnt, line4_info};
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

    state = MENU_STATE_IDLE;
    action = ACTION_STOPPED;

    btns = buttons::getInstance();
    display = lcd::getInstance();
    settings = nvm::getInstance();
    leds = LEDString::getInstance();

    bpm = settings->getBPM();

    // initial display
    display->writePString(LINE_1, (PGM_P)pgm_read_word(&(line1[LINE_HOME])));
    display->writePString(LINE_2, (PGM_P)pgm_read_word(&(line2[LINE_HOME])));
    display->writePString(LINE_3, (PGM_P)pgm_read_word(&(line3[LINE_HOME])));
    display->writePString(LINE_4, (PGM_P)pgm_read_word(&(line4[LINE_HOME])));

    this->updateBPMLine(true);
#ifdef DEBUG
    printf_P(PSTR("UI init'd\n"));
#endif
}

void ui::stateSetBPM()
{

}

void ui::stateIdle()
{
    if (btns->upOneShot() || btns->isUpHeld())
    {
        if (bpm < 240)
        {
            ++bpm;
            updateBPMLine(true);
            saveTime = WAIT_4_SAVE_TIME;
        }
    }
    else if (btns->downOneShot() || btns->isDownHeld())
    {
        if (bpm > 20)
        {
            --bpm;
            updateBPMLine(true);
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
        posn = 12;
    }

    snprintf(buffer, 20, "%3d", bpm);
    display->writeString(LINE_2, buffer, posn);
}

void ui::updateContrastLine()
{
    snprintf(buffer, 20, "%3d", display->getContrast());
    display->writeString(LINE_2, buffer, 11);
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

            if (btns->menuOneShot())
            {

            }
        }  break;

        case MENU_STATE_BPM:
        {

        }  break;

        case MENU_STATE_PATTERN:
        {

        }  break;

        case MENU_STATE_CONTRAST:
        {

        }  break;

        case MENU_STATE_INFO:
        {

        }  break;
    }

    if (saveTime)
    {
        --saveTime;
        if (!saveTime)
        {
            settings->setBPM(bpm);
            settings->saveNVM();
            printf_P(PSTR("Saved new BPM of %d\n"), bpm);
        }
    }
}