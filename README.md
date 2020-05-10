# solar-controller
Open Source solar hot water system controller

## src structure
* core: the core algorithms used by the controller and common to real embedded hardware, unit tests, and x86 sim
* hw: the hardware abstraction layer (esp32, 8-bit AVR, x86)
* sim: contains a simulator, i.e. runs the core and main program loop on x86
* tests: contains unit tests which exercise the algortihms in core

## Building from source
To build everything: `make`
To build only the AVR hex or only the simulator: `make -f Makefile.avr program`
or `make -f Makefile.sim program`
To flash the AVR hex using avrdude: `make -f Makefile.avr install`
