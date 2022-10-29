#ifndef SET_H
#define SET_H
#include <vector>
#include "Slot.h"

class Set
{
public:
    Set(uint32_t blocks, uint32_t bytes, bool write_alloc, bool write_thr, bool lru, uint32_t *cache_ctr, uint32_t *mem_ctr, uint32_t *miss_mem_ctr) : blocks(blocks),bytes(bytes),write_alloc(write_alloc),write_thr(write_thr),lru(lru),cache_ctr(cache_ctr),mem_ctr(mem_ctr),miss_mem_ctr(miss_mem_ctr)
    {
        std::vector<Slot> set = std::vector<Slot>();
        for (uint32_t x = 0; x < blocks; x++)
        {
            set.push_back(Slot(bytes, write_alloc, write_thr, lru));
        }
    }                                                                               
    bool is_hit(uint32_t tag, uint64_t current_ts);   // checks if block exists in cache
    void pull_mem(uint32_t tag, uint64_t current_ts); // used to pull memory from DRAM
    Slot *get_slot(uint32_t tag, uint64_t current_ts); // gets the correct slot

    // get method
    std::vector<Slot> getSet()
    {
        return set;
    }

private:
    uint32_t blocks;
    uint32_t bytes;
    uint32_t tag;
    bool write_alloc;
    bool write_thr;
    bool lru;
    uint32_t *cache_ctr;
    uint32_t *mem_ctr;
    uint32_t *miss_mem_ctr;
    std::vector<Slot> set; // vector of slots
};

#endif // SET_H