#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "cache_simulator.h"
#include "Cache.h"

/*
Big Todo:
    Understand what we store for accesses to the cache/dram
        WHAT EXACTLY ARE WE COUNTING
    Figure out write back overwrite code
    Implement
        write thr
        write bk
        write alloc
        no write alloc


*/

cache_simulator::~cache_simulator()
{
    delete this->cache;
    delete this->cache_ctr;
    delete this->mem_ctr;
    delete this->miss_mem_ctr;
}

cache_simulator::cache_simulator(int sets, int blocks, int bytes, bool write_alloc, bool write_thr, bool lru)
{
    //store all cache parameters
    this->sets = sets;
    this->blocks = blocks;
    this->bytes = bytes;
    this->write_alloc = write_alloc;
    this->write_thr = write_thr;
    this->lru = lru;
    //dynamically allocated cache and mem access counters to make counting cycles easy
    cache_ctr = new uint32_t;
    mem_ctr = new uint32_t;
    miss_mem_ctr = new uint32_t;
    *cache_ctr = 0;
    *mem_ctr = 0;
    *miss_mem_ctr = 0;
    cache = new Cache(sets, blocks, bytes, write_alloc, write_thr, lru, cache_ctr, mem_ctr, miss_mem_ctr);
}

void cache_simulator::printResult(std::vector<uint32_t> values)
{
    // loads, stores, ldr_hits, ldr_misses, str_hits, str_misses, total
    // have the lines below
    // std::cout << "Simulation Results: "
    //           << "\n";
    std::cout << "Total loads: " << values[0] << "\n";
    std::cout << "Total stores: " << values[1] << "\n";
    std::cout << "Load hits: " << values[2] << "\n";
    std::cout << "Load misses: " << values[3] << "\n";
    std::cout << "Store hits: " << values[4] << "\n";
    std::cout << "Store misses: " << values[5] << "\n";
    std::cout << "Total cycles: " << values[6] << "\n";
}

std::vector<uint32_t> cache_simulator::parseTraces()
{
    std::vector<uint32_t> results; // store info in vector
    // initalizes inputs
    uint32_t loads, stores, ldr_hits, ldr_misses, str_hits, str_misses, total;
    loads = 0;
    stores = 0;
    ldr_hits = 0;
    ldr_misses = 0;
    str_hits = 0;
    str_misses = 0;
    total = 0;

    // parse to find number of loads and stores first... get that working
    std::string line;
    std::string lOrS;
    int hOrM;
    uint32_t addr;
    std::string add;
    while (std::getline(std::cin, line))
    {
        //get operation type from the trace file
        std::istringstream ss(line); //read in total line
        ss >> lOrS; //store the first char (has to be l or s)
        //get mem address
        ss >> std::hex >> addr; //store the second word (hex address)
        //count total loads and stores
        if (lOrS.compare("l") == 0)
        {
            loads = loads + 1;
        }
        else
        {
            stores = stores + 1;
        }
        //access the mem address w/ the specified operation from trace
        hOrM = cache->access(addr, lOrS[0]);
        //increments correct data counter
        //based on hit or miss and load or store
        if (hOrM == 1 && lOrS.compare("l") == 0)
        {
            ldr_hits++;
        }
        if (hOrM == 1 && lOrS.compare("s") == 0)
        {
            str_hits++;
        }
        if (hOrM == 0 && lOrS.compare("l") == 0)
        {
            ldr_misses++;
        }
        if (hOrM == 0 && lOrS.compare("s") == 0)
        {
            str_misses++;
        }
    }
    total = (*cache_ctr) * 1 + (*mem_ctr) * 100 + (*miss_mem_ctr) * 100 * bytes/4; //total cycles
    // stores results to vector
    results.push_back(loads);
    results.push_back(stores);
    results.push_back(ldr_hits);
    results.push_back(ldr_misses);
    results.push_back(str_hits);
    results.push_back(str_misses);
    results.push_back(total);

    return results;
}
