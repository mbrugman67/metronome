/***************************************************
 * file: flashStrings.h
 ***************************************************
 * display strings that are stored in flash memory
 * to save RAM
 **************************************************/

#ifndef FLASH_STRINGS_H_
#define FLASH_STRINGS_H_

#include <avr/pgmspace.h>

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
const char line2_cnt[] PROGMEM = {"Contrast:           "};
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

#define getPStrPtr(x)    (PGM_P)pgm_read_word(&(x))

#endif // FLASH_STRINGS_H_