CC = g++
CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

csim: cache_simulator.o Cache.o Set.o Slot.o csim.o
	$(CC) -o csim csim.o cache_simulator.o Cache.o Set.o Slot.o

csim.o: csim.cpp
	$(CC) -c csim.cpp $(CFLAGS)

cache_simulator.o: cache_simulator.cpp cache_simulator.h Cache.h Set.h Slot.h
	$(CC) -c cache_simulator.cpp $(CFLAGS)

Cache.o: Cache.cpp Cache.h Set.h Slot.h
	$(CC) -c Cache.cpp $(CFLAGS)

Set.o: Set.cpp Set.h Slot.h
	$(CC) -c Set.cpp $(CFLAGS)

Slot.o: Slot.cpp Slot.h
	$(CC) -c Slot.cpp $(CFLAGS)

all: cache_simulator.o Cache.o Set.o Slot.o csim.o
	$(CC) -o csim csim.o cache_simulator.o Cache.o Set.o Slot.o

.PHONY: clean all
clean:
	rm -f *.o csim