#ifndef CACHE_SIMULATOR_H
#define CACHE_SIMULATOR_H
#include "Cache.h"
#include <vector>


class cache_simulator
{
private:
    /* data */
    uint32_t sets;
    uint32_t blocks;
    uint32_t bytes;
    bool write_alloc;
    bool write_thr;
    bool lru;
    uint32_t* mem_ctr;
    uint32_t* cache_ctr;
    Cache *cache;

public:
    cache_simulator(int sets, int blocks, int bytes, bool write_alloc, bool write_thr, bool lru);
    ~cache_simulator();
    void printResult(std::vector<int> values);
    std::vector<int> parseTraces();
};

#endif // CACHE_SIMULATOR_H
