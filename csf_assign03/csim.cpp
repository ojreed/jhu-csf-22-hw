#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "cache_simulator.h"
#include "Cache.h"

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

int main(int argc, char *argv[])
{
    // validate correct number of inputs
    if (argc != 7)
    {             // invalid number of arguments
        return 0; // process cant run
    }

    // TODO:: validate trace tookover CIN

    // convert input to strings for easy use
    std::string current_exec_name = argv[0]; // Name of the current exec program
    std::vector<std::string> params;
    if (argc > 1)
    {
        params.assign(argv, argv + argc);
    }

    // parse input for set block and byte size
    int sets, blocks, bytes;
    try
    {
        // pull inputs and convert string to ints
        sets = std::stoi(params[1]);
        blocks = std::stoi(params[2]);
        bytes = std::stoi(params[3]);
    }
    catch (const std::exception &e) // catch invalid input errors for sets, blocks, and bytes
    {
        std::cerr << e.what() << " Cache Size Parameter Not an Int" << '\n';
        return 0;
    }
    // validate power of 2 TODO: or should it allows 1's? who knows --> certainly not i
    if ((sets % 2 != 0) || (blocks % 2 != 0) || (bytes % 2 != 0))
    {
        std::cerr << "Input is not a power of two" << std::endl;
        return 0;
    }
    // validate pos
    if ((sets < 0) || (blocks < 0) || (bytes < 4))
    {
        std::cerr << "Input is not of valid size" << std::endl;
        return 0;
    }

    // parse input for cache usage modes
    bool write_alloc, write_thr, lru;
    // write alloc
    if (params[4].compare("write-allocate") == 0)
    {
        write_alloc = 1;
    }
    else if (params[4].compare("no-write-allocate") == 0)
    {
        write_alloc = 0;
    }
    else
    {
        std::cout << "Invalid Input Param for Write_Alloc" << std::endl;
        return 0;
    }
    // write through
    if (params[5].compare("write-through") == 0)
    {
        write_thr = 1;
    }
    else if (params[5].compare("write-back") == 0)
    {
        write_thr = 0;
    }
    else
    {
        std::cout << "Invalid Input Param for Write_Thr" << std::endl;
        return 0;
    }
    // lru
    if (params[6].compare("lru") == 0)
    {
        lru = 1;
    }
    else if (params[6].compare("fifo") == 0)
    {
        lru = 0;
    }
    else
    {
        std::cout << "Invalid Input Param for LRU" << std::endl;
        return 0;
    }

    // test inputs
    std::cout << "Validate Inputs: " << "\n";
    std::cout << "sets: " << sets << "\n";
    std::cout << "blocks: " << blocks << "\n";
    std::cout << "bytes: " << bytes << "\n";
    std::cout << "write_alloc: " << write_alloc << "\n";
    std::cout << "write_thr: " << write_thr << "\n";
    std::cout << "lru: " << lru << "\n";

    cache_simulator csim(sets, blocks, bytes, write_alloc, write_thr, lru);
    csim.printResult(csim.parseTraces());
    return 1;
}