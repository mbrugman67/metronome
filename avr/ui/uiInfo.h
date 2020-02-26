/***************************************************
 * file: uiInfo.h
 ***************************************************
 * Derived menu class
 * This will display info
 **************************************************/

#ifndef UI_INFO_H_
#define UI_INFO_H_

#include "uiBase.h"

class uiInfo : public uiBase
{
public:
    uiInfo() : uiBase() {}
    ~uiInfo() {}

    void update(bool& change);
    void start();
    PGM_P getName();

private:
};

#endif // UI_INFO_H_