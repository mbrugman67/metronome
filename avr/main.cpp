/*
 */
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

#include "project.h"
#include "sys/hardware.h"
#include "sys/serialPrintf.h"

#include "nvm/nvm.h"
#include "lcd/lcd.h"
#include "string/LEDString.h"
#include "buttons/buttons.h"
#include "ui/ui.h"

static volatile uint8_t isrMilliseconds = 0;
static volatile uint8_t isrLastMilleseconds = 0;

uint32_t milliseconds = 0;

int main(void)
{
    uint8_t taskNum = 0;

#ifdef DEBUG
    cfgSerial(S_BAUD_115200, S_FORMAT_8N1);

    // set stdin and stdout to go through the serial port
    FILE uart_str; 
    uart_str.put = USART0SendByte;
    uart_str.get = USART0ReceiveByte;
    uart_str.flags = _FDEV_SETUP_RW;
    stdout = stdin = &uart_str;
#endif

    // user interface drives it all
    ui interface;
    interface.init();

    // init hardware
    setupIO();
    setupTimer1();
    setupTimer2();
    
    // singleton instance of LCD handler,
    // LED string, and pushbutton handler classes
    lcd* display = lcd::getInstance();
    //LEDString* string = LEDString::getInstance();
    buttons* hdwr = buttons::getInstance();
    nvm* eeprom = nvm::getInstance();

    setupWatchdog();
    sei();

#ifdef DEBUG
    printf_P(PSTR("Starting metronome main loop, boot count %d\r\n"),
        eeprom->getBootCount());
#endif

    uint16_t blinkyTimer = 0;
    uint8_t count = 0;
    uint8_t seconds = 0;
    uint8_t minutes = 0;
    uint16_t last_ms = (milliseconds % 1000);
    char timestring[21];
    const char* HEADER_LINE = "MNU   UP   DN   ENTR";
    
    strncpy(timestring, HEADER_LINE, 20);

    display->clearAll();
    display->writeString(LINE_1, timestring);
    display->writeChar(LINE_2, 0xc2, 16); 
    display->writeString(LINE_2, "Hi Sweetie! ", 4);
    display->writeString(LINE_3, "Elapsed: ");
    
    snprintf(timestring, 20, "Contrast: %03d", display->getContrast());
    display->writeString(LINE_4, timestring);

    while (true)
    {
        switch (taskNum)
        {
            case 0:
            {
                ++blinkyTimer;
                if (blinkyTimer < (667 / TASK_INTERVAL))
                {
                    LED_L_SET(true);
                }
                else if (blinkyTimer < (1000 / TASK_INTERVAL))
                {
                    LED_L_SET(false);
                }
                else
                {
                    blinkyTimer = 0;
                }

            }  break;

            case 1:
            {
                hdwr->update();
            }  break;
            
            case 2:
            {
                uint16_t msnow = (uint16_t)milliseconds % 1000;
                if (last_ms > msnow)
                {
                    ++seconds;
                }
                last_ms = msnow;

                if (seconds == 60)
                {
                    ++minutes;
                    seconds = 0;
                }

                if (!(++count % 32))
                {
                    snprintf(timestring, 20, "%02d:%02d.%01d",
                        minutes, seconds, msnow / 100);
                    
                    display->writeString(LINE_3, timestring, strlen("Elapsed: "));
                }
                //string->update();
            }  break;
            
            case 3:
            {
                uint16_t contrast = display->getContrast();

                if (hdwr->upOneShot() && contrast < 200)
                {
                    display->setContrast(++contrast);
                    snprintf(timestring, 20, "Contrast: %03d     ", display->getContrast());
                    display->writeString(LINE_4, timestring);
                }
                if (hdwr->downOneShot() && display > 0)
                {
                    display->setContrast(--contrast);
                    snprintf(timestring, 20, "Contrast: %03d     ", display->getContrast());
                    display->writeString(LINE_4, timestring);
                }
                //interface.update();

                //if (interface.stopAll())                string->stop();
                //else if (interface.startMetronome())    string->start(interface.getBPM());
                //else if (interface.startPretty())       string->pretty();


            }  break;
        }

        ++taskNum;
        taskNum %= TASK_INTERVAL;

        // do busy stuff in the dead time between the end of one
        // task and the start of the next
        while (isrMilliseconds == isrLastMilleseconds)
        {
            // busy
        }

        // global millisecond counter
        uint8_t incr = isrMilliseconds - isrLastMilleseconds;
        isrLastMilleseconds = isrMilliseconds;
        milliseconds += (uint32_t)(incr) & 0xff;
        
        wdt_reset();
    }

    return 0;
}

/*****************************************************
* System tick ISR.  Once each millisecond
*****************************************************/
ISR(TIMER2_COMPA_vect, ISR_BLOCK)
{
    ++isrMilliseconds;
    PIN_IO2_TGL();
}
