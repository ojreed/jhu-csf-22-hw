CC = g++
CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

csim: cache_simulator.o
	$(CC) -o csim cache_simulator.o

cache_simulator.o: cache_simulator.cpp cache_simulator.h
	$(CC) -c cache_simulator.cpp $(CFLAGS)

.PHONY: clean all
clean:
	rm -f *.o csim