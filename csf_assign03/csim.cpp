#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <math.h>
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
/*
error checker
Before starting the simulation, your simulator should check to make sure that the simulation parameters are reasonable. Examples of invalid configuration parameters include (but are not limited to):

    block size is not a power of 2
    number of sets is not a power of 2
    block size is less than 4
    write-back and no-write-allocate were both specified

If the configuration parameters are invalid, the program should

    Print an error message to stderr or std::cerr, and
    Exit with a non-zero exit code

*/

// helper function to determine if parameter is a power of two
int is_power_of_two(int x) 
{
    if(ceil(log2(x)) != floor(log2(x))) 
    {
        return 1;
    }
    return 0;
}

int validate_bools(std::vector<std::string> params,bool *write_alloc,bool *write_thr,bool *lru)
{
    int validCombo = 0;
    // set correct cache settings based on parameters
    // write alloc
    if (params[4].compare("write-allocate") == 0)
    {
        *write_alloc = 1;
    }
    else if (params[4].compare("no-write-allocate") == 0)
    {
        *write_alloc = 0;
        validCombo = 1;
    }
    else
    {
        std::cerr << "Invalid Input Param for Write_Alloc" << std::endl;
        return 1;
    }
    // write through
    if (params[5].compare("write-through") == 0)
    {
        *write_thr = 1;
    }
    else if (params[5].compare("write-back") == 0)
    {
        if(validCombo == 1) { // special error check
            std::cerr << "Cannot use no-write-allocate and write-back together" << std::endl;
            return 1;
        }
        *write_thr = 0;
    }
    else
    {
        std::cerr << "Invalid Input Param for Write_Thr" << std::endl;
        return 1;
    }
    // lru
    if (params[6].compare("lru") == 0)
    {
        *lru = 1;
    }
    else if (params[6].compare("fifo") == 0)
    {
        *lru = 0;
    }
    else
    {
        std::cerr << "Invalid Input Param for LRU" << std::endl;
        return 1;
    }
    return 0;
}

int validate_ints(std::vector<std::string> params, int *sets, int *blocks, int *bytes) 
{
    try
    {
        // pull inputs and convert string to ints
        *sets = std::stoi(params[1]);
        *blocks = std::stoi(params[2]);
        *bytes = std::stoi(params[3]);
    }
    catch (const std::exception &e) // catch invalid input errors for sets, blocks, and bytes
    {
        std::cerr << e.what() << " Cache Size Parameter Not an Int" << '\n';
        return 1;
    }
    // validate power of 2
    if (is_power_of_two(*sets) || is_power_of_two(*blocks) || (is_power_of_two(*bytes) && *bytes >= 4))
    {
        std::cerr << "Input is not a power of two" << std::endl;
        return 1;
    }
    // validate pos
    if ((*sets <= 0) || (*blocks <= 0) || (*bytes < 4))
    {
        std::cerr << "Input is not of valid size" << std::endl;
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // validate correct number of inputs
    if (argc != 7)
    {   // invalid number of arguments
        std::cerr << " Wrong number of parameters" << '\n';
        return 1; // process cant run
    }
    // convert input to strings for easy use
    std::string current_exec_name = argv[0]; // Name of the current exec program
    std::vector<std::string> params;
    if (argc > 1)
    {
        params.assign(argv, argv + argc);
    }
    // parse input for set block and byte size
    int sets, blocks, bytes;
    int invalid = validate_ints(params, &sets, &blocks, &bytes);
    if (invalid == 1) {
        return 1;
    }

    // parse input for cache usage modes
    bool write_alloc, write_thr, lru;
    invalid = validate_bools(params, &write_alloc, &write_thr, &lru);
    if (invalid == 1) {
        return 1;
    }
    
    //c reate a cache_simulator class with the validated parameters
    cache_simulator csim(sets, blocks, bytes, write_alloc, write_thr, lru);
    // print out the results from parsing and executing the trace file
    std::vector<uint32_t> parsed = csim.parseTraces();
    csim.printResult(parsed);
    return 0;
}