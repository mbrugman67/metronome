#include "../project.h"

#define EEPROM_SIGNATURE_WORD   0x54fa

struct CONFIG xConfig;

void initEEPROM(void)
{   
    size_t cfgSize = sizeof(struct CONFIG);
    
    // get from eepromSize
    eeprom_read_block((void*)&xConfig, (void*)0x100, cfgSize);
    
    // check to see if signature matches; if not, we'll have to set default values
    if(xConfig.signature != EEPROM_SIGNATURE_WORD)
    {
        printf("Invalid nonvol - resetting\r\n");
        memset((void*)&xConfig, 0, cfgSize);
        
        strcpy(xConfig.SSID, "Default SSID");
        strcpy(xConfig.password, "Default Password");
        
        xConfig.signature = EEPROM_SIGNATURE_WORD;
    }
    
    ++xConfig.resetCount;
    
    eeprom_update_block((void*)&xConfig, (void*)0x100, cfgSize);
}

void saveEEPROM(void)
{
    eeprom_update_block((void*)&xConfig, (void*)0x100, sizeof(struct CONFIG));
}