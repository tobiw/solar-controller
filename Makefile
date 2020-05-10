all:
	make -f Makefile.avr program
	make -f Makefile.sim program

clean:
	make -f Makefile.avr clean
	make -f Makefile.sim clean
