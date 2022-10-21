// Set.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Set.h"
#include "Slot.h"

// default constructor
Set::Set(int blocks, int bytes, bool write_alloc, bool write_thr, bool lru)//, uint32_t* cache_ctr,uint32_t* mem_ctr)
{
    this->blocks = blocks;
    this->bytes = bytes;
    this->write_alloc = write_alloc;
    this->write_thr = write_thr;
    this->lru = lru;
    this->tag = tag;
    cache_ctr = cache_ctr,
    mem_ctr = mem_ctr;
    for (int x = 0; x < blocks; x++)
    {
        set.push_back(Slot(bytes, write_alloc, write_thr, lru));
    }
}

bool Set::is_hit(uint32_t tag, uint32_t offset, uint32_t current_ts)
{
    for (int i = 0; i < set.size(); i++)
    {
        if (this->set[i].getTag() == tag && this->set[i].is_valid())
        {
            if (this->lru)
            {
                set[i].setTS(current_ts);
            } // if fifo we don't need to modify
            return true;
        }
    }
    return false;
}

Slot* Set::get_slot(uint32_t tag, uint32_t offset)
{
    for (int i = 0; i < set.size(); i++)
    {
        if (this->set[i].getTag() == tag && this->set[i].is_valid())
        {
            return &set[i];
        }
    }
    return NULL;
}

void Set::pull_mem(uint32_t tag, uint32_t index, uint32_t offset, uint32_t current_ts)
{
    // pull from mem and put into cache
    // add a block to the set in the correct slot by the current rule set, find correct slot and replace
    // for correct set, update the tag and the time stamp
    // lru- based on last used time
    // fifo- based on load order

    uint32_t least_recent_ts = set[0].getTS();
    Slot *least_recent_slot = &set[0];
    // find correct slot to replace
    for (int x = 1; x < blocks; x++)
    {
        if (set[x].getTS() <= least_recent_ts)
        {
            least_recent_slot = &set[x];
            least_recent_ts = set[x].getTS();
        }
    }
    if ((*least_recent_slot).is_diff_from_mem() && !write_thr) {//if write back we write back on overload
        //TODO: Modify Memory
        *mem_ctr++; //increment the number of accesses to mem --> for write back
    }
    *mem_ctr++; //increment the number of accesses to mem --> for pull new
    (*least_recent_slot).setTag(tag);
    (*least_recent_slot).setTS(current_ts);
    (*least_recent_slot).set_valid(true);
    (*least_recent_slot).set_diff_from_mem(false);
}