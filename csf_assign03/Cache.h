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
    int split_address(uint32_t address, uint32_t* tag, uint32_t* index, uint32_t* offset);
    uint64_t current_ts;
    // more stuff

    std::vector<Set> getCache()
    {
        return cache;
    }

private:
    // stuff
    std::vector<Set> cache; // vector of sets
    uint32_t sets;
    uint32_t blocks;
    uint32_t bytes;
    uint32_t tag;
    bool write_alloc;
    bool write_thr;
    bool lru;
    uint32_t *cache_ctr;
    uint32_t *mem_ctr;
};

#endif // CACHE_H