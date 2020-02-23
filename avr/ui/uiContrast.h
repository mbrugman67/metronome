
#ifndef UI_CONTRAST_H_
#define UI_CONTRAST_H_

#include "uiBase.h"

class uiContrast : public uiBase
{
public:
    uiContrast() {}
    ~uiContrast() {}

    void update(bool& change);
    void start();

private:
    uint16_t contrast;

    void updateContrastLine();
};

#endif // UI_CONTRAST_H_