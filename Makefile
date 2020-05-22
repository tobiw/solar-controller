all:
	make -f Makefile.avr main_avr.hex
	make -f Makefile.sim sim

test:
	cd src/tests && make clean && make

clean:
	make -f Makefile.avr clean
	make -f Makefile.sim clean
	cd src/tests && make clean
