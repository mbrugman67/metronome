/***************************************************
 * file: uiBpm.h
 ***************************************************
 * Derived menu class
 * This will adjust the BPM setting
 **************************************************/

#ifndef UI_BPM_H_
#define UI_BPM_H_

#include "uiBase.h"

class uiBpm : public uiBase
{
public:
    uiBpm() : uiBase() {}
    ~uiBpm() {}

    void update(bool& change);
    void start();
    PGM_P getName();

private:
    uint16_t bpm;

    void updateBPMLine();
};

#endif // UI_BPM_H_