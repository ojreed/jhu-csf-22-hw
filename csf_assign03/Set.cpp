// Set.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Set.h"
#include "Slot.h"

// default constructor
Set::Set(int blocks, int bytes, bool write_alloc, bool write_thr, bool lru)
{
    this->blocks = blocks;
    this->bytes = bytes;
    this->write_alloc = write_alloc;
    this->write_thr = write_thr;
    this->lru = lru;
    this->tag = tag;
    for (int x = 0; x<blocks;x++) {
      set.push_back(Slot(bytes,write_alloc,write_thr,lru));
    }
}

bool Set::is_hit(uint32_t tag, uint32_t offset) {
    for(int i = 0; i < set.size(); i++) {
        if(set[i].getTag() == tag) {
            return true;
        }
    }
    return false;
}

void Set::pull_mem(uint32_t tag, uint32_t index, uint32_t offset){
    //pull from mem and put into cache
    //add a block to the set in the correct slot by the current rule set, find correct slot and replace 
    //for correct set, update the tag and the time stamp 
    for(std::vector<Slot>::iterator it = set.begin(); it != set.end(); ++it) {
        if(this->lru == lru) {

        } else { //fifo

        }
    }

    return;
}