/*
 */
#include "project.h"

// Assign stdin and stdout streams to our functions that handle serial port
static FILE usart0stdio = FDEV_SETUP_STREAM(USART0SendByte, USART0ReceiveByte, _FDEV_SETUP_RW);

static volatile uint8_t isrMilliseconds = 0;
static volatile uint8_t isrLastMilleseconds = 0;

uint32_t milliseconds = 0;

int main(void)
{
    uint8_t taskNum = 0;

    cfgSerial(S_BAUD_115200, S_FORMAT_8N1);
    stdin=stdout=&usart0stdio;
    
    setupIO();
    setupTimer2();
    initEEPROM(); 
    printf_P(PSTR("\r\nAVR Metronome %ld\r\n\r\n"), xConfig.resetCount);  

    setupWatchdog();
    sei();

    printflash("Starting metronome main loop\r\n");

    uint16_t blinkyTimer = 0;

    while (true)
    {
        switch (taskNum)
        {
            case 0:
            {
                ++blinkyTimer;
                if (LED_L_GET() && blinkyTimer >= (666 / TASK_INTERVAL))
                {
                    LED_L_OFF();
                    blinkyTimer = 0;
                } else if (!LED_L_GET() && blinkyTimer >= (333 / TASK_INTERVAL))
                {
                    LED_L_ON();
                    blinkyTimer = 0;
                }

            }  break;

            case 1:
            {

            }  break;
            
            case 2:
            {

            }  break;
            
            case 3:
            {

            }  break;
        }

        ++taskNum;
        taskNum %= TASK_INTERVAL;

        while (isrMilliseconds == isrLastMilleseconds)
        {
            // busy
        }

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
}
