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
    enum led_mode_t
    {
        MODE_METRO,
        MODE_PULSE,
        MODE_WHQ,
    };

    LEDString();
    ~LEDString() {}

    void clear();
    void start(led_mode_t m, uint16_t bpm);
    void stop();

    void update();
    
private:

    WS2812 leds;
    cRGB pixel;

    bool running;
    bool draw;
    
    bool movingRight;
    led_mode_t mode;
    uint16_t posn;
    uint16_t ticksPerMove;
    uint16_t moveTicks;
    uint32_t lastTickCount;

    void startMetronome(uint16_t bpm);
    void startWHQ(uint16_t bpm);
    void startPulse(uint16_t bpm);

    void metronome();
    void pulse();
    void WHQ();
};

#endif // STRING_H_