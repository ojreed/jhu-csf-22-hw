#ifndef CACHE_H
#define CACHE_H
#include "Set.h"

class Cache
{

public:
    Cache();
    Cache(int sets, int blocks, int bytes, bool write_alloc, bool write_thr, bool lru, uint32_t *cache_ctr, uint32_t *mem_ctr): sets(sets), blocks(blocks), bytes(bytes), write_alloc(write_alloc),write_thr(write_thr),lru(lru),cache_ctr(cache_ctr),mem_ctr(mem_ctr)
    {
        for (int x = 0; x < sets; x++)
        {
            cache.push_back(Set(blocks, bytes, write_alloc, write_thr, lru, cache_ctr, mem_ctr));
        }

    }; // default constructor
    int access(uint32_t adddress, char instruction);
    int load(uint32_t adddress);
    int store(uint32_t adddress);
    uint32_t current_ts;
    // more stuff

    std::vector<Set> getCache()
    {
        return cache;
    }

private:
    // stuff
    std::vector<Set> cache; // vector of sets
    int sets;
    int blocks;
    int bytes;
    int tag;
    bool write_alloc;
    bool write_thr;
    bool lru;
    uint32_t *cache_ctr;
    uint32_t *mem_ctr;
};

#endif // CACHE_H