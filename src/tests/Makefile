TEST_OBJ=test_sensors.o test_wiznet.o test.o
DEP_OBJ=../core/sensors.o ../core/wiznet.o ../hw/mock.o
CFLAGS=-Wall -g -I../../include -DUNIT_TEST
LDFLAGS=-lcunit -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

testrunner: $(DEP_OBJ) $(TEST_OBJ)
	gcc -o testrunner $^ $(LDFLAGS)
	./testrunner

clean:
	rm -f testrunner $(TEST_OBJ) $(DEP_OBJ)
