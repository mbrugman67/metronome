
#include <stdlib.h>

#include "buttons.h"

buttons* buttons::_inst = NULL;

buttons* buttons::getInstance()
{
    if (!_inst)
    {
        _inst = (buttons*)malloc(sizeof(buttons));
    }

    return (_inst);
}

