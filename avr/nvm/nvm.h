/***************************************************
 * file: nvm.h
 ***************************************************
 * handle non-vol memory.  Parameters, settings, 
 * and such will be stored in EEPROM
 **************************************************/
#ifndef NVM_H_
#define NVM_H_

#include <stdint.h>

#define EEPROM_SIGNATURE    0xCAFE
#define DEFAULT_CONTRAST    40
#define DEFAULT_BPM         60
#define DEFAULT_MODE         1

class nvm
{
public:
    static nvm* getInstance();

    void saveNVM();
    bool loadNVM();
    void setDefaults();

    uint16_t getSignature()         { return (data.signature); }
    uint16_t getNVMVersion()        { return (data.version); }
    uint16_t getBootCount()         { return (data.bootCount); }
    uint16_t getContrast()          { return (data.contrast); }
    uint16_t getBPM()               { return (data.bpm); } 
    uint16_t getMode()              { return (data.mode); }

    void setContract(uint16_t c)    { data.contrast = c; }
    void setBPM(uint16_t b)         { data.bpm = b; }
    void setMode(uint16_t m)        { data.mode = m; }

private:
    nvm() {}
    virtual ~nvm() {}
    static nvm* _inst;

    struct nvm_store_t
    {
        uint16_t    signature;
        uint16_t    version;
        uint16_t    bootCount;
        uint16_t    contrast;
        uint16_t    bpm;
        uint16_t    mode;
    };

    nvm_store_t data;
};

#endif // NVM_H_