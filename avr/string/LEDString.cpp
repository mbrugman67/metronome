#include <stdlib.h>

#include <avr/io.h>

#include "../project.h"

#ifdef DEBUG
#include <stdio.h>
#include <avr/pgmspace.h>
#endif

LEDString::LEDString() : leds()
{
    leds.setOutput(&PORTD, &DDRD, PIND4);
    lastTickCount = getTickCount();
}

void LEDString::start(led_mode_t m, uint16_t b)
{
    switch ((uint16_t)m)
    {
        case MODE_METRO:        this->startMetronome(b);
        case MODE_PULSE:        this->startPulse(b);
        case MODE_WHQ:          this->startWHQ(b);
    }

    mode = m;
}

void LEDString::startPulse(uint16_t bpm)
{

}

void LEDString::startWHQ(uint16_t bpm)
{
    
}

void LEDString::startMetronome(uint16_t bpm)
{
    // so, the active LED should move from one end of the
    // string to the other at bpm rate (beats/minute)
    // this all depends on the length of the string and
    // how often the update method gets hit
    // 
    // it takes 45ms to update the whole string
    uint32_t rate = (uint32_t)(59955 / TASK_INTERVAL) / (uint32_t)bpm;
    ticksPerMove = (uint16_t)rate / STRING_LENGTH;

    moveTicks = 0;
    posn = 0;
    movingRight = true;
    running = true;

#ifdef DEBUG
    printf_P(PSTR("Starting metronome at %d BPM, %d updates/move\n"),
        bpm, ticksPerMove);
#endif
}

void LEDString::stop()
{
    running = false;
}

void LEDString::clear()
{
    pixel.r = 0; 
    pixel.g = 0;
    pixel.b = 0;

    for (size_t ii = 0; ii < STRING_LENGTH; ++ii)
    {
        leds.set_crgb_at(ii, pixel);
    }

    draw = true;
}

void LEDString::pulse()
{

}

void LEDString::WHQ()
{

}

void LEDString::metronome()
{
    if ((++moveTicks) >= ticksPerMove)
    {
        this->clear();
        moveTicks = 0;
        draw = true;

        pixel.r = 255;
        pixel.g = 80;
        pixel.b = 0;
        leds.set_crgb_at(posn, pixel);

        if (movingRight)
        {
            if (posn < (STRING_LENGTH - 1))
            {
                ++posn;
            }
            else
            {
                --posn;
                movingRight = false;
            }
        }
        else
        {
            if (posn > 0)
            {
                --posn;
            }
            else
            {
                ++posn;
                movingRight = true;
            }
        }
    }
}

void LEDString::update()
{   
    if (running)
    {
        switch (mode)
        {
            case MODE_METRO:    this->metronome();
            case MODE_PULSE:    this->pulse();
            case MODE_WHQ:      this->WHQ();
        }
    }
    
    if (draw)
    {  
        draw = false;
        leds.sync();
    }
}