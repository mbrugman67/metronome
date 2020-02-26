#include <stdlib.h>
#include <string.h>

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

void LEDString::start()
{
    uint16_t bpm = settings.getBPM();
    mode = (led_mode_t)settings.getMode();

    switch (mode)
    {
        case MODE_METRO:        this->startMetronome(bpm);    break;
        case MODE_PULSE:        this->startPulse(bpm);        break;
        case MODE_WHQ:          this->startWHQ(bpm);          break;
        case MODE_SAMPLE:       break;
    }
}

void LEDString::startSample()
{
    running = true;
    clr.setRed(0);
    clr.setGrn(0);
    clr.setBlu(0);

    mode = MODE_SAMPLE;
}

void LEDString::startPulse(uint16_t bpm)
{
    // so, the active LED should move from one end of the
    // string to the other at bpm rate (beats/minute)
    // this all depends on the length of the string and
    // how often the update method gets hit
    // 
    // it takes 45ms to update the whole string
    //uint32_t rate = (uint32_t)(59955 / TASK_INTERVAL) / (uint32_t)bpm;
    // manually tweaked for correct rate
    uint32_t rate = (uint32_t)(62000 / TASK_INTERVAL) / (uint32_t)bpm;
    ticksPerMove = (uint16_t)rate;
    secondaryTicks = ticksPerMove / 6;

    moveTicks = 0;
    running = true;

#ifdef DEBUG
    printf_P(PSTR("Starting pulse at %d BPM, %d updates/cycle\n"),
        bpm, ticksPerMove);
#endif
}

void LEDString::startWHQ(uint16_t bpm)
{
    // so, the active LED should move from one end of the
    // string to the other at bpm rate (beats/minute)
    // this all depends on the length of the string and
    // how often the update method gets hit
    // 
    // it takes 45ms to update the whole string
    //uint32_t rate = (uint32_t)(59955 / TASK_INTERVAL) / (uint32_t)bpm;
    // manually tweaked for correct rate
    uint32_t rate = (uint32_t)(62000 / TASK_INTERVAL) / (uint32_t)bpm;
    ticksPerMove = (uint16_t)rate / 4;
    secondaryTicks = ticksPerMove / 2;
    cycle = 0;
    running = true;

#ifdef DEBUG
    printf_P(PSTR("Starting WHQ at %d BPM, %d updates/cycle\n"),
        bpm, ticksPerMove);
#endif
}

void LEDString::startMetronome(uint16_t bpm)
{
    // so, the active LED should move from one end of the
    // string to the other at bpm rate (beats/minute)
    // this all depends on the length of the string and
    // how often the update method gets hit
    // 
    // it takes 45ms to update the whole string
    //uint32_t rate = (uint32_t)(59955 / TASK_INTERVAL) / (uint32_t)bpm;
    // manually tweaked for correct rate
    uint32_t rate = (uint32_t)(64000 / TASK_INTERVAL) / (uint32_t)bpm;
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
    this->clear();
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

void LEDString::sample()
{
    if (settings.getSweepClr()->getRed() != clr.getRed() ||
        settings.getSweepClr()->getGrn() != clr.getGrn() ||
        settings.getSweepClr()->getBlu() != clr.getBlu())
    {
        memcpy((void*)&clr, (void*)settings.getSweepClr(), sizeof(ledclr));
        this->clear();

        pixel.r = clr.getRed();
        pixel.g = clr.getGrn();
        pixel.b = clr.getBlu();

        for (size_t ii = 2; ii < STRING_LENGTH - 2; ++ii)
        {
            leds.set_crgb_at(ii, pixel);
        }
    
        draw = true;
    }
}

void LEDString::pulse()
{
    ++moveTicks;

    if (moveTicks < secondaryTicks)
    {
        pixel.r = settings.getSweepClr()->getRed();
        pixel.g = settings.getSweepClr()->getGrn();
        pixel.b = settings.getSweepClr()->getBlu();

        for (size_t ii = 0; ii < STRING_LENGTH; ++ii)
        {
            leds.set_crgb_at(ii, pixel);
        }

        draw = true;
    }
    else
    {
        this->clear();

        draw = true;
    }

    if (moveTicks >= ticksPerMove)
    {
        moveTicks = 0;
    }
}

void LEDString::WHQ()
{
    if ((++moveTicks) >= ticksPerMove)
    {
        moveTicks = 0;

        this->clear();

        pixel.r = settings.getSweepClr()->getRed();
        pixel.g = settings.getSweepClr()->getGrn();
        pixel.b = settings.getSweepClr()->getBlu();

        ++cycle;
        // every quarter note
        leds.set_crgb_at(9, pixel);
        leds.set_crgb_at(10, pixel);

        // half every other
        if (!(cycle % 2))
        {
            leds.set_crgb_at(5, pixel);
            leds.set_crgb_at(6, pixel);
        }

        // whole every 4th
        if (!(cycle % 4))
        {
            leds.set_crgb_at(1, pixel);
            leds.set_crgb_at(2, pixel);
        }

        draw = true;
    }
    if (moveTicks == secondaryTicks)
    {
        this->clear();
        draw = true;
    }
}

void LEDString::metronome()
{
    if ((++moveTicks) >= ticksPerMove)
    {
        this->clear();
        moveTicks = 0;
        draw = true;

        pixel.r = settings.getSweepClr()->getRed();
        pixel.g = settings.getSweepClr()->getGrn();
        pixel.b = settings.getSweepClr()->getBlu();
        
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
            case MODE_METRO:    this->metronome();  break;
            case MODE_PULSE:    this->pulse();      break;
            case MODE_WHQ:      this->WHQ();        break;
            case MODE_SAMPLE:   this->sample();     break;
        }
    }
    
    if (draw)
    {  
        draw = false;
        leds.sync();
    }
}