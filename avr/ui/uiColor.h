/***************************************************
 * file: uiColor.h
 ***************************************************
 * Derived menu class
 * This will adjust the color settings
 **************************************************/

#ifndef UI_COLOR_H_
#define UI_COLOR_H_

#include "uiBase.h"

class uiColor : public uiBase
{
public:
    uiColor() : uiBase() {}
    ~uiColor() {}

    void update(bool& change);
    void start();
    PGM_P getName();

private:
    enum set_color_t
    {
        RED,
        GREEN,
        BLUE,
        DONE 
    };

    uint16_t thisColor;
    ledclr clr;
    set_color_t cstate;

    void updateColorLine();
};

#endif // UI_COLOR_H_