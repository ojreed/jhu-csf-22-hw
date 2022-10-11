CC = g++
CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

csim: cache_simulator.o Cache.o Set.o Slot.o
	$(CC) -o csim csim.cpp cache_simulator.o Cache.o Set.o Slot.o

cache_simulator.o: cache_simulator.cpp cache_simulator.h
	$(CC) -c cache_simulator.cpp $(CFLAGS)

Cache.o: Cache.cpp Cache.h
	$(CC) -c Cache.cpp $(CFLAGS)

Set.o: Set.cpp Set.h
	$(CC) -c Set.cpp $(CFLAGS)

Slot.o: Slot.cpp Slot.h
	$(CC) -c Slot.cpp $(CFLAGS)

all: cache_simulator.o
	$(CC) -o csim cache_simulator.o

.PHONY: clean all
clean:
	rm -f *.o csim