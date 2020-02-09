#include <stdlib.h>

#include "LEDString.h"

LEDString* LEDString::_inst = NULL;

LEDString* LEDString::getInstance()
{
    if (!_inst)
    {
        _inst = (LEDString*)malloc(sizeof(LEDString));
    }

    return (_inst);
}