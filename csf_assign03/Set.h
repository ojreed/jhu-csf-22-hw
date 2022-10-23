#ifndef SET_H
#define SET_H
#include "Slot.h"

class Set
{
public:
    Set(uint32_t blocks, uint32_t bytes, bool write_alloc, bool write_thr, bool lru, uint32_t *cache_ctr, uint32_t *mem_ctr) : blocks(blocks),bytes(bytes),write_alloc(write_alloc),write_thr(write_thr),cache_ctr(cache_ctr),mem_ctr(mem_ctr)
    {
        for (int x = 0; x < blocks; x++)
        {
            set.push_back(Slot(bytes, write_alloc, write_thr, lru));
        }
    }                                                                                  // default constructor
    bool is_hit(uint32_t tag, uint32_t offset, uint64_t current_ts);                   // checks if block exists in cache
    void pull_mem(uint32_t tag, uint32_t index, uint32_t offset, uint64_t current_ts); // used to pull memory from DRAM
    Slot *get_slot(uint32_t tag, uint32_t offset, uint64_t current_ts);                                     // gets the correct slot

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
    std::vector<Slot> set; // vector of slots
};

#endif // SET_H