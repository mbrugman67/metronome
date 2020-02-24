/***************************************************
 * file: uiMode.h
 ***************************************************
 * Derived menu class
 * This will adjust the metronome mode setting
 **************************************************/

#ifndef UI_MODE_H_
#define UI_MODE_H_

#include "uiBase.h"

class uiMode : public uiBase
{
public:
    uiMode() : uiBase() {}
    ~uiMode() {}

    void update(bool& change);
    void start();
    PGM_P getName();

private:
    uint16_t mode;

    void updateModeLine(uint16_t m);
};

#endif // UI_MODE_H_