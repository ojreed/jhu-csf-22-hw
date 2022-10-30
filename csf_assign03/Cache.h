#ifndef CACHE_H
#define CACHE_H
#include "Set.h"

class Cache
{

public:
    Cache();
    // constructor in initalizer list format to fix issue of shadowing pointers
    Cache(int sets, int blocks, int bytes, bool write_alloc, bool write_thr, bool lru, uint32_t *cache_ctr): sets(sets), blocks(blocks), bytes(bytes), write_alloc(write_alloc),write_thr(write_thr),lru(lru),cache_ctr(cache_ctr)
    {
        // creates a vector of sets with the correct number of sets
        for (int x = 0; x < sets; x++)
        {
            cache.push_back(Set(blocks, bytes, write_alloc, write_thr, lru, cache_ctr));
        }

    }; 
    int access(uint32_t adddress, char instruction); // usable function to parse weather we load or store
    int load(uint32_t adddress); // load a value from memory
    int store(uint32_t adddress); // write a value to memory
    int split_address(uint32_t address, uint32_t* tag, uint32_t* index, uint32_t* offset); // helper function to split an add into tag, index, offset
    uint64_t current_ts; // stores the current timestamp

    // geter helper function
    std::vector<Set> getCache()
    {
        return cache;
    }

private:
    // actual cache of sets
    std::vector<Set> cache; // vector of sets
    // cache params
    uint32_t sets;
    uint32_t blocks;
    uint32_t bytes;
    uint32_t tag;
    bool write_alloc;
    bool write_thr;
    bool lru;
    // data counters
    uint32_t *cache_ctr;
};

#endif // CACHE_H