/***************************************************
 * file: nvm.cpp
 ***************************************************
 * handle non-vol memory.  Parameters, settings, 
 * and such will be stored in EEPROM
 **************************************************/

#include "nvm.h"
#include "../project.h"

#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>
#include <avr/pgmspace.h>
#endif

#include <avr/eeprom.h>

nvm* nvm::_inst = NULL;

nvm* nvm::getInstance()
{
    if (!nvm::_inst)
    {
        nvm::_inst = (nvm*)malloc(sizeof(nvm));

        _inst->loadNVM();
    }

    return (_inst);
}

void nvm::setDefaults()
{
    data.signature = EEPROM_SIGNATURE;
    data.bootCount = 0;
    data.contrast  = DEFAULT_CONTRAST;
    data.bpm       = DEFAULT_BPM;
    data.mode      = DEFAULT_MODE;
}

void nvm::saveNVM()
{
    eeprom_update_block((void*)&data, (void*)0x100, sizeof(data));
}

bool nvm::loadNVM()
{
    bool retVal = true;

    eeprom_read_block((void*)&data, (void*)0x100, sizeof(data));

    if (data.signature != EEPROM_SIGNATURE)
    {
#ifdef DEBUG
        printf_P(PSTR("Invalid EEPROM signature! 0x%04x\n"),
            data.signature);
#endif

        this->setDefaults();
        retVal = false;
    }

    ++data.bootCount;
    this->saveNVM();

    return (retVal);
}
