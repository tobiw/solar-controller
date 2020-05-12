all:
	make -f Makefile.avr program
	make -f Makefile.sim program

test:
	cd src/tests && make

clean:
	make -f Makefile.avr clean
	make -f Makefile.sim clean
	cd src/tests && make clean
