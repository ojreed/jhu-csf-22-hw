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
    //todo: write code to actually pull from dram
    return;
}