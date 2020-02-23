#ifndef STRING_H_
#define STRING_H_

#include <stdint.h>

#include "../project.h"
#include "../sys/ws2812/WS2812.h"
#include "../sys/ws2812/cRGB.h"
#include "../sys/ioDefinitions.h"

class LEDString
{
public:
    static LEDString* getInstance();

    void clear();
    void start(uint16_t bpm);
    void stop();
    void pretty();

    void update();
    
private:
    LEDString() : leds() {}
    virtual ~LEDString() {}

    WS2812 leds;
    cRGB pixel;

    static LEDString* _inst;
    static bool initDone;
    static bool running;
    bool draw;
    
    bool movingRight;
    bool doPretty;
    uint16_t posn;
    uint16_t ticksPerMove;
    uint16_t moveTicks;
    uint32_t lastTickCount;

    void init();
    void metronome();
    void pattern();
};

#endif // STRING_H_