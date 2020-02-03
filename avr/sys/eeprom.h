#ifndef _EEPROM_H_
#define _EEPROM_H_

// variable addresses (all values are word size)
struct CONFIG
{
    uint32_t    resetCount;
    char        SSID[128];
    char        password[128];
    uint16_t    signature;
};

extern struct CONFIG xConfig;

void initEEPROM(void);
void saveEEPROM(void);

#endif //define _EEPROM_H_