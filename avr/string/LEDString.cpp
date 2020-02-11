#include <stdlib.h>

#include <avr/io.h>

#include "LEDString.h"

#ifdef DEBUG
#include <stdio.h>
#include <avr/pgmspace.h>
#endif

LEDString* LEDString::_inst = NULL;
bool LEDString::initDone = false;
bool LEDString::running = false;

LEDString* LEDString::getInstance()
{
    if (!_inst)
    {
        _inst = (LEDString*)malloc(sizeof(LEDString));
    }

    return (_inst);
}

void LEDString::init()
{
    if (!initDone)
    {
        initDone = true;

        leds.setOutput(&PORTD, &DDRD, PIND2);
        leds.setColorOrderRGB();
    }
}

void LEDString::start(uint16_t bpm)
{
    // so, the active LED should move from one end of the
    // string to the other at bpm rate (beats/minute)
    // this all depends on the length of the string and
    // how often the update method gets hit
    uint32_t rate = (uint32_t)(60000 / TASK_INTERVAL) / (uint32_t)bpm;
    ticksPerMove = (uint16_t)rate / STRING_LENGTH;

    tickCount = 0;
    posn = 0;
    movingRight = true;
    running = true;
    doPretty = false;

#ifdef DEBUG
    printf_P(PSTR("Starting metronome at %d BPM, %d updates/move\n"),
        bpm, ticksPerMove);
#endif
}

void LEDString::stop()
{
    running = false;
    doPretty = false;
}

void LEDString::pretty()
{
    running = false;
    doPretty = true;
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
}

void LEDString::metronome()
{
    this->clear();

    if (tickCount >= ticksPerMove)
    {
        ticksPerMove = 0;

        pixel.r = 255;
        pixel.g = 255;
        pixel.b = 255;
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

void LEDString::pattern()
{
    ;
}

void LEDString::update()
{
    ++tickCount;
    
    if (running)
    {
        this->metronome();
    }
    else if (doPretty)
    {
        this->pattern();
    }
    else
    {
        this->clear();
    }
    
    //cli();
    //leds.sync();
    //sei();
}