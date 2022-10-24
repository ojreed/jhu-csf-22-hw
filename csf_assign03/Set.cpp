// Set.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Set.h"
#include "Slot.h"

bool Set::is_hit(uint32_t tag, uint64_t current_ts) // check if a block w/ correct info exists
{
    for (uint32_t i = 0; i < set.size(); i++) // loop through all blocks
    {
        if (this->set[i].getTag() == tag && this->set[i].is_valid()) // if is valid and correct tag then we have a hit
        {
            if (this->lru) // lru mode needs to update timestamp
            {
                set[i].setTS(current_ts);
            }            // if fifo we don't need to modify
            return true; // hit
        }
    }
    return false; // mis
}

Slot *Set::get_slot(uint32_t tag, uint64_t current_ts) // same as is hit but returns the correct block
{
    for (uint32_t i = 0; i < set.size(); i++) // loop through all blocks
    {
        if (this->set[i].getTag() == tag && this->set[i].is_valid()) // find the correct valid tag
        {
            if (this->lru) // if LRU update ts
            {
                set[i].setTS(current_ts);
            }               // if fifo we don't need to modify
            return &set[i]; // block found
        }
    }
    return NULL; // NO BLOCK EXISTS
}

void Set::pull_mem(uint32_t tag, uint64_t current_ts)
{
    // pull from mem and put into cache
    // add a block to the set in the correct slot by the current rule set, find correct slot and replace
    // for correct set, update the tag and the time stamp
    // lru- based on last used time
    // fifo- based on load order

    // base case of first block
    uint32_t least_recent_ts = set[0].getTS();
    Slot *least_recent_slot = &set[0];
    // find correct slot to replace
    for (uint32_t x = 1; x < blocks; x++)
    {
        if (set[x].is_valid() == false)
        { // if we find and unused block just use that and stop search
            least_recent_slot = &set[x];
            least_recent_ts = set[x].getTS();
            x = blocks;
        }
        else if (set[x].getTS() < least_recent_ts) // if we find an older block than the current one
        {
            least_recent_slot = &set[x]; // make the older block the current best replace block
            least_recent_ts = set[x].getTS();
        }
    }
    if ((*least_recent_slot).is_diff_from_mem() && (*least_recent_slot).is_valid() && !write_thr)
    {                 // if write back we write back on overload
        (*cache_ctr)+=(bytes/4*100); // increment the number of accesses to mem --> for write back
    }
    (*cache_ctr)+=(bytes/4*100);
    // if (hOrM == 1)
    // {
    //     (*miss_mem_ctr)++;
    // } // increment the number of accesses to mem --> for pull new} //miss
    // else
    // {
    //     (*mem_ctr)++;
    // } // increment the number of accesses to mem --> for pull new}

    (*least_recent_slot).setTag(tag);
    (*least_recent_slot).setTS(current_ts);
    (*least_recent_slot).set_valid(true);
    (*least_recent_slot).set_diff_from_mem(false);
}