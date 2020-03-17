import argparse

'''
timers.py

Use this to quickly figure out which timer you may want to use.

Parameters:
    -r - update rate, in milliseconds.  This is the time the 
         timer will run, for example if in CTC mode.
         Defaulut value is 1.0
    -f - CPU frequency in Megahertz.  Default is 16.0
    -p - AVR part.  Choices are:
           328p - ATMega328p, typical Arduino part
           32u4 - ATMega32u4, used in Leonardo
           644 - ATMega644, one that I use in other projects

Output will be a list of available prescalers and count values
that match for each timer for that combination of part, frequency,
and update time.  Output will also contain TCCxB register values 
for the prescaler

One useful piece of the output is the "error" value.  The output
will give a count for the timer preset value, but the "perfect" 
preset may not be an integer.  The suggested value is rounded, 
and the "error" is the deviation from desired update rate.

NOTE: The info here is what I took from the datasheets.  No 
guarantee is given to accuracy; always refer to datasheets if
there's any questions

Feel free to add more AVR models
'''

class ATMega328p():
    '''
    Timer register definitions for the Atmel AVR ATMeaga328p
    '''
    def __init__(self):
        self.name = 'atmega328p'
        self.timers = []
 
        timer0 = Timer('Timer0', 8)
        register0 = Timer.TimerRegister('TCCR0B')
        register0.addScalerAndSetting(1,    '((1 << CS00))')
        register0.addScalerAndSetting(8,    '((1 << CS01))')
        register0.addScalerAndSetting(64,   '((1 << CS01) | (1 << CS00))')
        register0.addScalerAndSetting(256,  '((1 << CS02))')
        register0.addScalerAndSetting(1024, '((1 << CS02) | (1 << CS00))')
        timer0.addRegister(register0)
        self.timers.append(timer0)
 
        timer1 = Timer('Timer1', 16)
        register1 = Timer.TimerRegister('TCCR1B')
        register1.addScalerAndSetting(1,    '((1 << CS10))')
        register1.addScalerAndSetting(8,    '((1 << CS11))')
        register1.addScalerAndSetting(64,   '((1 << CS11) | (1 << CS10))')
        register1.addScalerAndSetting(256,  '((1 << CS12))')
        register1.addScalerAndSetting(1024, '((1 << CS12) | (1 << CS10))')
        timer1.addRegister(register1)
        self.timers.append(timer1)
 
        timer2 = Timer('Timer2', 8)
        register2 = Timer.TimerRegister('TCCR2B')
        register2.addScalerAndSetting(1,    '((1 << CS20))')
        register2.addScalerAndSetting(8,    '((1 << CS21))')
        register2.addScalerAndSetting(32,   '((1 << CS21) | (1 << CS20))')
        register2.addScalerAndSetting(64,   '((1 << CS22)')
        register2.addScalerAndSetting(128,  '((1 << CS22) | (1 << CS20))')
        register2.addScalerAndSetting(256,  '((1 << CS22) | (1 << CS21))')
        register2.addScalerAndSetting(1024, '((1 << CS22) | (1 << CS21) | (1 << CS20))')
        timer2.addRegister(register2)
        self.timers.append(timer2) 
 
class ATMega32u4():
    '''
    Timer register definitions for the Atmel AVR ATMeaga32u4
    '''
    def __init__(self):
        self.name = 'atmega32u4'
        self.timers = []
 
        timer0 = Timer('Timer0', 8)
        register0 = Timer.TimerRegister('TCCR0B')
        register0.addScalerAndSetting(1,    '((1 << CS00))')
        register0.addScalerAndSetting(8,    '((1 << CS01))')
        register0.addScalerAndSetting(64,   '((1 << CS01) | (1 << CS00))')
        register0.addScalerAndSetting(256,  '((1 << CS02))')
        register0.addScalerAndSetting(1024, '((1 << CS02) | (1 << CS00))')
        timer0.addRegister(register0)
        self.timers.append(timer0)
 
        timer1 = Timer('Timer1', 16)
        register1 = Timer.TimerRegister('TCCR1B')
        register1.addScalerAndSetting(1,    '((1 << CS10))')
        register1.addScalerAndSetting(8,    '((1 << CS11))')
        register1.addScalerAndSetting(64,   '((1 << CS11) | (1 << CS10))')
        register1.addScalerAndSetting(256,  '((1 << CS12))')
        register1.addScalerAndSetting(1024, '((1 << CS12) | (1 << CS10))')
        timer1.addRegister(register1)
        self.timers.append(timer1)
 
        timer3 = Timer('Timer3', 16)
        register3 = Timer.TimerRegister('TCCR3B')
        register3.addScalerAndSetting(1,    '((1 << CS30))')
        register3.addScalerAndSetting(8,    '((1 << CS31))')
        register3.addScalerAndSetting(64,   '((1 << CS31) | (1 << CS30))')
        register3.addScalerAndSetting(256,  '((1 << CS32))')
        register3.addScalerAndSetting(1024, '((1 << CS32) | (1 << CS30))')
        timer3.addRegister(register3)
        self.timers.append(timer3) 
 
        timer4 = Timer('Timer4', 10)
        register4 = Timer.TimerRegister('TCCR4B')
        register4.addScalerAndSetting(1,     '((1 << CS40))')
        register4.addScalerAndSetting(2,     '((1 << CS41))')
        register4.addScalerAndSetting(4,     '((1 << CS41) | (1 << CS40))')
        register4.addScalerAndSetting(8,     '((1 << CS42))')
        register4.addScalerAndSetting(16,    '((1 << CS42) | (1 << CS40))')
        register4.addScalerAndSetting(32,    '((1 << CS42) | (1 << CS41))')
        register4.addScalerAndSetting(64,    '((1 << CS42) | (1 << CS42) | (1 << CS40))')
        register4.addScalerAndSetting(128,   '((1 << CS43))')
        register4.addScalerAndSetting(256,   '((1 << CS43) | (1 << CS40))')
        register4.addScalerAndSetting(512,   '((1 << CS43) | (1 << CS41))')
        register4.addScalerAndSetting(1024,  '((1 << CS43) | (1 << CS41) | (1 << CS40))')
        register4.addScalerAndSetting(2048,  '((1 << CS43) | (1 << CS42))')
        register4.addScalerAndSetting(4096,  '((1 << CS43) | (1 << CS42) | (1 << CS40))')
        register4.addScalerAndSetting(8192,  '((1 << CS43) | (1 << CS42) | (1 << CS41))')
        register4.addScalerAndSetting(16384, '((1 << CS43) | (1 << CS42) | (1 << CS41) | (1 << CS40))')
        timer4.addRegister(register4)
        self.timers.append(timer4) 

class ATMega644():
    '''
    Timer register definitions for the Atmel AVR ATMeaga644
    '''
    def __init__(self):
        self.name = 'atmega644'
        self.timers = []
 
        timer0 = Timer('Timer0', 8)
        register0 = Timer.TimerRegister('TCCR0B')
        register0.addScalerAndSetting(1,    '((1 << CS00))')
        register0.addScalerAndSetting(8,    '((1 << CS01))')
        register0.addScalerAndSetting(64,   '((1 << CS01) | (1 << CS00))')
        register0.addScalerAndSetting(256,  '((1 << CS02))')
        register0.addScalerAndSetting(1024, '((1 << CS02) | (1 << CS00))')
        timer0.addRegister(register0)
        self.timers.append(timer0)
 
        timer1 = Timer('Timer1', 16)
        register1 = Timer.TimerRegister('TCCR1B')
        register1.addScalerAndSetting(1,    '((1 << CS10))')
        register1.addScalerAndSetting(8,    '((1 << CS11))')
        register1.addScalerAndSetting(64,   '((1 << CS11) | (1 << CS10))')
        register1.addScalerAndSetting(256,  '((1 << CS12))')
        register1.addScalerAndSetting(1024, '((1 << CS12) | (1 << CS10))')
        timer1.addRegister(register1)
        self.timers.append(timer1)
 
        timer2 = Timer('Timer2', 8)
        register2 = Timer.TimerRegister('TCCR2B')
        register2.addScalerAndSetting(1,    '((1 << CS20))')
        register2.addScalerAndSetting(8,    '((1 << CS21))')
        register2.addScalerAndSetting(32,   '((1 << CS21) | (1 << CS20))')
        register2.addScalerAndSetting(64,   '((1 << CS22)')
        register2.addScalerAndSetting(128,  '((1 << CS22) | (1 << CS20))')
        register2.addScalerAndSetting(256,  '((1 << CS22) | (1 << CS21))')
        register2.addScalerAndSetting(1024, '((1 << CS22) | (1 << CS21) | (1 << CS20))')
        timer2.addRegister(register2)
        self.timers.append(timer2) 

class Timer():
    '''
    Timer class
    A Timer is really a collection of registers.  Each registrer
    has a collection prescalers (value and bit settings)
    '''
    class TimerRegister():
        '''
        A register has a name (e.g., TCCR1B) and a collection
        of scaler settings.  A scaler setting is a Tuple of
        (Prescaler <int>, Bitvalues <string>)
        '''
        def __init__(self, psRegName):
            '''
            Instantiate with a register name (e.g., "TCCR1B")
            '''
            self.name = psRegName
            self.scalerSettings = []
       
        def addScalerAndSetting(self, value, bits):
            '''
            Add a set of prescaler setting, which consists
            of prescaler value and a string describing bit
            settings
            '''
            tmp = (value, bits)
            self.scalerSettings.append(tmp)
 
    def __init__(self, name, resolution):
        '''
        Instantiate the Timer class with the name (e.g. "Timer0")
        and the timer resolution in bits
        '''
        self.name = name;
        self.resolution = resolution
        self.registers = []
 
    def addRegister(self, reg):
        '''
        Add a register object
        '''
        self.registers.append(reg)
 
class TimerCalc():
    '''
    Do the actual calculations and output formatting
    '''
    def __init__(self, avr, interval, cpuFreq = 16000000):
        '''
        Instantiate the class with the AVR object, desired
        timer interval (in milliseconds), and CPU frequency
        (in Hertz, not Megahertz)
        '''
        self.timers = avr.timers
        self.interval = interval
        self.cpuFreq = cpuFreq
        self.frequency = (interval / 1000)**-1
 
    def _calcError(self, actual, desired):
        '''
        Utility to calculate percentage error from a
        given value
        '''
        if desired != 0:
            return ((actual - desired) / desired)
        else:
            return (1)
 
    def _getPrintLineStr(self, prescaler, resolution, register, bits):
        '''
        Returns a formatted string:
            PRESCALER, CALCULATED PRESET COUNT, ERROR %, REGISTER SETTING
        '''
        # Caluclate the actual count preset.  This will be a floating
        # point value
        actualCount = self.interval / (self.cpuFreq / 1000 / prescaler)**-1

        # Round that value to the nearest integer and set type to integer
        # Ugh, I hate Python
        intCount = int(round(actualCount, 0))

        # Calculate the error
        error = self._calcError(intCount, actualCount)
 
        # If the count value ends up being zero, or is larger
        # that the resolution of the timer, then return
        # an empty string.  Otherwise, format it out
        if intCount > 0 and intCount < 2**resolution:
            return ('   {:>4}    {:>5}  {:>+6.2%}  {} = {};'
                .format(prescaler, intCount, error, register, bits))
        else:
            return (None)
   
    def _getPrintHeaderStr(self):
        '''
        just a static header string
        '''
        return (' Prescale  count   error  register setting')
 
    def printTimer(self, timer):
        '''
        Print the settings for a timer.  This would be the name of the
        timer, the resolution, and the valid combinations of prescaler
        and preset count
        '''
        print ('{} ({}-bit resolution):'.format(timer.name, timer.resolution))
        print (self._getPrintHeaderStr())

        # each register in the timer
        for register in timer.registers:
            # get the tuple of scalerSettings.
            # Ugh, I hate python
            for (prescaler, bits) in register.scalerSettings:
                s = self._getPrintLineStr(prescaler, timer.resolution, register.name, bits)
                if s != None:
                    print (s)
 
    def printAllTimers(self):
        '''
        Zoom through all of the timers on the AVR and print the stuffs
        for each one
        '''
        for timer in self.timers:
            self.printTimer(timer)
            print (' ')
 
 #############################################################
 #### main() #################################################
 #############################################################

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description = 'Cipher out timer settings')
 
    parser.add_argument('-f', '--cpuFreq', nargs = '?', default = 16.0, type = float, 
                        help = 'CPU Frequency in MHz, not Hz.  Default = 16.0')
    parser.add_argument('-r', '--rate', nargs = '?', default = 1.0, type = float,
                        help = 'Timer update interval time in milliseconds.  Default = 1.0')
    parser.add_argument('-p', '--part', nargs = '?', default = '328p',
                        help = 'AVR part, 328p, 32u4, 644.  Default 328p')
 
    args = parser.parse_args()
 
    # Pick the part
    avr = None
    if args.part == '328p':
        avr = ATMega328p()
    elif args.part == '32u4':
        avr = ATMega32u4()
    elif args.part == '644':
        avr = ATMega644()
    else:
        print ('unknown part "{}"'.format(args.part))
        quit()
 
    cipherTimer = TimerCalc(avr, args.rate, args.cpuFreq * 1000000)
 
    print ('Calculating timer register settings for a {} running at {:>.3f} MHz'
        .format(avr.name, cipherTimer.cpuFreq / 1000000))
   
    print ('Desired timer update rate is {:>.3f} millisconds ({:>.3f}KHz)'.
            format(cipherTimer.interval, cipherTimer.frequency / 1000.0))
    print (' ')
 
    cipherTimer.printAllTimers()
 