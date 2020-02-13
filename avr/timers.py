import argparse
 
class ATMega328p():
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
 
 
class Timer():
    class TimerRegister():
        def __init__(self, psRegName):
            self.name = psRegName
            self.scalerSettings = []
       
        def addScalerAndSetting(self, value, bits):
            tmp = (value, bits)
            self.scalerSettings.append(tmp)
 
    def __init__(self, name, resolution):
        self.name = name;
        self.resolution = resolution
        self.registers = []
 
    def addRegister(self, reg):
        self.registers.append(reg)
 
class TimerCalc():
    def __init__(self, avr, interval, cpuFreq = 16000000):
        self.timers = avr.timers
        self.interval = interval
        self.cpuFreq = cpuFreq
        self.frequency = (interval / 1000)**-1
 
 
    def _calcError(self, actual, desired):
        if desired > 0:
            return ((actual - desired) / desired)
        else:
            return (1)
 
    def _getPrintLineStr(self, prescaler, resolution, register, bits):
        actualCount = self.interval / (self.cpuFreq / 1000 / prescaler)**-1
        intCount = int(round(actualCount, 0))
        error = self._calcError(intCount, actualCount)
 
        if intCount > 0 and intCount < 2**resolution:
            return ('   {:>4}    {:>5}  {:>+6.2%}  {} = {};'
                .format(prescaler, intCount, error, register, bits))
        else:
            return (None)
   
    def _getPrintHeaderStr(self):
        return (' Prescale  count   error  register setting')
 
    def printTimer(self, timer):
        print '{} ({}-bit resolution):'.format(timer.name, timer.resolution)
        print self._getPrintHeaderStr()
        for register in timer.registers:
            for (prescaler, bits) in register.scalerSettings:
                s = self._getPrintLineStr(prescaler, timer.resolution, register.name, bits)
                if s != None:
                    print s
 
    def printAllTimers(self):
        for timer in self.timers:
            self.printTimer(timer)
            print ' '
 
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description = 'Cipher out timer settings')
 
    parser.add_argument('-f', '--cpuFreq', nargs = '?', default = 16.0, type = float)
    parser.add_argument('-r', '--rate', nargs = '?', default = 1.0, type = float)
    parser.add_argument('-p', '--part', nargs = '?', default = '328p')
 
    args = parser.parse_args()
 
    rate = args.rate
    freq = args.cpuFreq
    part = args.part
 
    avr = None
    if args.part == '328p':
        avr = ATMega328p()
    else:
        print 'unknown part "{}"'.format(args.part)
        quit()
 
    cipherTimer = TimerCalc(avr, args.rate, args.cpuFreq * 1000000)
 
    print ('Calculating timer register settings for a {} running at {:>.3f} MHz'
        .format(avr.name, cipherTimer.cpuFreq / 1000000))
   
    print ('Desired timer update rate is {:>.3f} millisconds ({:>.3f}KHz)'.
            format(cipherTimer.interval, cipherTimer.frequency / 1000.0))
    print ' '
 
    cipherTimer.printAllTimers()
 