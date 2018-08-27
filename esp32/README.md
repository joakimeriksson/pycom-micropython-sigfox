MicroPython port for the ESP32 based boards from Pycom
======================================================

In order to build this project, a copy of the Espressif IDF is required and it's
path must be specified via the IDF_PATH variable. See the Makefile for details.

The modified Espressif IDF that we use to build this port can be found in:
https://github.com/pycom/pycom-esp-idf

Build instructions
------------------

First build the mpy-cross compiler:

    $ cd ../mpy-cross
    $ make all

After that, build the ESP32 port for one of Pycom boards (first the bootloader, then the app):

    $ cd ../esp32
    $ make BOARD=LOPY -j5 TARGET=boot
    $ make BOARD=LOPY -j5 TARGET=app

Flash the board (connect P2 to GND and reset before starting):

    $ make BOARD=LOPY -j5 flash

Using frozen modules
--------------------

Place all the python scripts that you'd like to be frozen into the flash memory of the board inside
the 'frozen' folder in the esp32 directory. Then build as indicated before.


/// JE
Adding modules

* Add to the esb32/mods file (a file.c at least - see micropython hello-world tutorial)
* Add to the esb32 mpconfigport.h
* Add to the application.mk

