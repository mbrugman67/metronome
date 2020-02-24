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
#define DEFAULT_MODE         0
#define DEFAULT_RGB         {0xff, 0x80, 0x00}

class ledclr
{
public:
    ledclr() {}
    ledclr(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(b) {}
    ~ledclr() {}

    uint8_t getRed()            { return(red); }
    uint8_t getGrn()            { return(green); }
    uint8_t getBlu()            { return(blue); }

    void setRed(uint8_t r)      { red = r; }
    void setGrn(uint8_t g)      { green = g; }
    void setBlu(uint8_t b)      { blue = b; }

private:
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t align;
};

class nvm
{
public:
    nvm() {}
    ~nvm() {}
    
    void saveNVM();
    bool loadNVM();
    void setDefaults();

    uint16_t getSignature()         { return (data.signature); }
    uint16_t getNVMVersion()        { return (data.version); }
    uint16_t getBootCount()         { return (data.bootCount); }
    uint16_t getContrast()          { return (data.contrast); }
    uint16_t getBPM()               { return (data.bpm); } 
    uint16_t getMode()              { return (data.mode); }
    ledclr*  getSweepClr()          { return (&data.sweepClr); }
    ledclr*  getPulseClr()          { return (&data.pulseClr); }
    ledclr*  getWholeClr()          { return (&data.wholeClr); }
    ledclr*  getHalfClr()           { return (&data.halfClr); }
    ledclr*  getQtrClr()            { return (&data.qtrClr); }

    void setContrast(uint16_t c)    { data.contrast = c; }
    void setBPM(uint16_t b)         { data.bpm = b; }
    void setMode(uint16_t m)        { data.mode = m; }


private:
    struct nvm_store_t
    {
        uint16_t    signature;
        uint16_t    version;
        uint16_t    bootCount;
        uint16_t    contrast;
        uint16_t    bpm;
        uint16_t    mode;
        ledclr      sweepClr;
        ledclr      pulseClr;
        ledclr      wholeClr;
        ledclr      halfClr;
        ledclr      qtrClr;
    };

    nvm_store_t data;
};

#endif // NVM_H_