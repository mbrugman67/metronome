# The over-engineered metronome
I was looking for a new project using only parts I had at home.  I'm also learning guitar, so I suddenly got the idea of makeing a metronome.

## Status - oh, not even close to being done ;)

### What the heck is it?
A string of addressible LEDs will give a visual timing indicator used to help with guitaring.  Being over-engineered, it will have some pushbuttons and an LCD display.  Because why not?

### Parts
So, the main constraint was to only use parts in my bin-o-junk (and whatever structure that gets 3D printed).  I picked:  
+ Metro Mini Arduino-type board.  It's an ATMega 328p on a small board with a USB-to-serial chip on it.  I ordered a bunch from [Adafruit](https://www.adafruit.com/product/2590) a couple of years back and some left over.
+ a 4 x 20 LCD display with .  I have 2 in the junk bin; I seem to remember buying them scratch 'n' dent from Sparkfun 5 or more years agao.  I don't see them on the website anymore, not even in the [retired](https://www.sparkfun.com/categories/51?filter_option%5Bsubcategory%5D%5B%5D=category_76&filter_price_floor=&filter_price_ceil=&filter_option%5Bretired%5D%5B%5D=is_retired) section.  They seem to be a little brain-dead; some commands don't seem to work propery
+ some WS2812 addressible LEDs.  Got spools of 'em
+ 4 small mechanical limit switches

### Project layout
There are 3 main directories under the base `metronome` directory:  
+ avr - this contains all of the code for the ATMega chip
  + the base directory has the main.cpp file, a common header, and a python script.  The script is used to determine register setting for timers
  + buttons - a class that manages the 4 pushbuttons.  Debounce, press and hold, one-shot on press, etc
  + lcd - a class for the 4x20 display.  As I mentioned, the units in my junk bin seem to be brain damaged as a few commands don't work
  + string - a class for the string of addressible LEDS
  + sys - common files to config the serial port, I/O, fuses, etc.
    + There's a directory under here for the WS2812 library.  That's the only thing I didn't write, it came from Matthias Riegler.  It's a nice simple library that works well.
  + ui - human interface; ties pushbuttons, display, and LEDs together.  Really, drives the state of the whole thing
  + ref - reference material like datasheets and schematic of the proto board
+ doc - generic documentation
+ model - 3D model

There's also directories `obj` and `bin` created when it's built.  `bin` is where the final hex and other outputs are placed, `obj` is where the object files 
### Tooling
I like to use Linux and Mac OSX for development; I did this one is on OSX.
##### Development
I have never used the Arduino thingy, just never saw the need for it.  I don't like the look of the libraries.  I used Brew to install avr-gcc and avrdude.  For reference, the avr-gcc I used is based on GCC 7.2.0.

Editing is done in MS VS Code; I really like it as an editor.  It is the same look and feel on Mac OSX and Linux, and has a nice selection of plugins available.

There's a `Makefile` that will work for Linux, you may just need to change the first two lines to correctly point to your installation.  Windows people are on their own.

For debugging, I'm using a combination of debug print statements and a logic analyzer (and winky lights).  The print statements are sent out the USB-to-Serial link and can just be read in a serial terminal.

*When building for release, comment out the line that adds -DDEBUG to the CFLAGS.  It'll pull out all of the serial and print stuff, which will cut a large hunk out of the compiled hex file*
##### 3D design
The 3D modeling is all done in OpenSCAD.  The OpenSCAD files are in the repo, but I didn't include the gcode from the slicer because that's pretty dependent on the specific printer being used.
##### Diagrams, etc
Diagrams are in DrawIO.  It's free, and something I'm just learning to use.

### Notes to read if you've only used the Arduino IDE Chingus Thingy
If you're coming from the Arduino IDE... Well; I've never used it because I don't like that kind of simplification of things. It's a good way for people to learn, but they don't necessarily learn the 'real' way things work. If you want to know what's under the hood, there are two pieces of documentation that you absolutely must look at.

The first is the docs for avr libc. The compiler we're going to use as a gnu thing, avr-gcc, but what it really needs to work is a standard 'c' library to link to. One that is made to work on the AVR microcontrollers. The documentation for it is here. Don't read from start to end, but take a look. You'll see which headers need to be used for which functions and get a good overview of the toolchain.

The next thing is the datasheet for the AVR. Now that Microchip has purchased Atmel, I hope they keep up the good documentation that Atmel had... The nearest I can see with a quick glance is here. Work your way through the basics and focus on the peripherals. Without the Arduino environment to help you, you'll need to understand how to use the peripherals; how their registers work and such.