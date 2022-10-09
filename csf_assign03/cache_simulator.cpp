#include <iostream>
#include <string>

/*
Main function of program --> will manage input of command line arguments as well as initalizing the simulation
    Inputs:
        number of sets in the cache (a positive power-of-2)
        number of blocks in each set (a positive power-of-2)
        number of bytes in each block (a positive power-of-2, at least 4)
        write-allocate or no-write-allocate
        write-through or write-back
        lru (least-recently-used) or fifo evictions
*/
int main(int argc, char *argv[]){ 
    int sets = *argv[0];
    int blocks = *argv[1];
    int bytes = *argv[2];
    bool write_alloc[20] = (*argv[3].compare("write-allocate")) ? true : false;
    return 0;
}