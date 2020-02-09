#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "../sys/ioDefinitions.h"

class buttons
{
public:
    static buttons* getInstance();

private:
    buttons() {}
    virtual ~buttons() {}

    static buttons* _inst;
};

#endif // BUTTONS_H_