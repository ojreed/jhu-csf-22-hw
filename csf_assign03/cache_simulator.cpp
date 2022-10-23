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
}

cache_simulator::cache_simulator(int sets, int blocks, int bytes, bool write_alloc, bool write_thr, bool lru)
{
    this->sets = sets;
    this->blocks = blocks;
    this->bytes = bytes;
    this->write_alloc = write_alloc;
    this->write_thr = write_thr;
    this->lru = lru;
    cache_ctr = new uint32_t;
    mem_ctr = new uint32_t;
    // *cache_ctr = (uint32_t) 0;
    // *mem_ctr = (uint32_t) 0;
    *cache_ctr = 0;
    *mem_ctr = 0;
    cache = new Cache(sets, blocks, bytes, write_alloc, write_thr, lru, cache_ctr, mem_ctr);
}

void cache_simulator::printResult(std::vector<uint32_t> values)
{
    // loads, stores, ldr_hits, ldr_misses, str_hits, str_misses, total
    // have the lines below
    std::cout << "Simulation Results: "
              << "\n";
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
        // something like this below, finish later
        std::istringstream ss(line);
        // std::getline(ss, lOrS, ' ');
        ss >> lOrS; // maybe will store first char
        ss >> std::hex >> addr;
        // std::cout<<addr<<std::endl;
        // addr = std::stoi(add.substr(2,8),nullptr,16);
        if (lOrS.compare("l") == 0)
        {
            loads = loads + 1;
        }
        else
        {
            stores = stores + 1;
        }
        hOrM = cache->access(addr, lOrS[0]);
        // std::cout<<hOrM<<std::endl;
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
    total = (*cache_ctr) * 1 + (*mem_ctr) * 100 * bytes/4;
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

/*
Goal:
Total loads: 318197
Total stores: 197486
Load hits: 314171
Load misses: 4026
Store hits: 188047
Store misses: 9439
Total cycles: 9845283

Current:
Total loads: 318197
Total stores: 197486
Load hits: 216821
Load misses: 101376
Store hits: 151658
Store misses: 45828
Total cycles: 84226344



*/