
#ifndef UI_IDLE_H_
#define UI_IDLE_H_

#include "uiBase.h"

class uiIdle : public uiBase
{
public:
    uiIdle() {}
    ~uiIdle() {}
    
    void update(bool& change);
    void start();
    PGM_P getName();

private:
    bool running;
};

#endif // UI_IDLE_H_