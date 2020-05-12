TEST_OBJ=test_sensors.o
DEP_OBJ=../core/sensors.o
CFLAGS=-Wall -g -I../../include
LDFLAGS=-lcunit -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

testrunner: $(DEP_OBJ) $(TEST_OBJ)
	gcc -o testrunner $^ $(LDFLAGS)
	./testrunner

clean:
	rm -f testrunner $(TEST_OBJ) $(DEP_OBJ)
