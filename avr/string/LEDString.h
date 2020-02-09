#ifndef STRING_H_
#define STRING_H_

#include <stdint.h>
#include "../sys/ioDefinitions.h"

class LEDString
{
public:
    LEDString* getInstance();

    void start(uint16_t bpm);
    void stop();
    void pretty();

    void update();
    
private:
    LEDString() {}
    virtual ~LEDString() {}

    static LEDString* _inst;
    uint16_t ticksPerMove;
};

#endif // STRING_H_