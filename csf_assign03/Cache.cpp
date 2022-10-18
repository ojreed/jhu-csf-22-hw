// Cache.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include "Cache.h"
#include "Set.h"

// default constructor
Cache::Cache(int sets, int blocks, int bytes, bool write_alloc, bool write_thr, bool lru)
{
   this->sets = sets;
   this->blocks = blocks;
   this->bytes = bytes;
   this->write_alloc = write_alloc;
   this->write_thr = write_thr;
   this->lru = lru;
   this->tag = tag;
   this->current_ts=0;
   for (int x = 0; x<sets;x++) {
     cache.push_back(Set(blocks,bytes,write_alloc,write_thr,lru));
   }
}

//helper function to make calling of load and store directly from trace simpler
int Cache::access(uint32_t adddress,char instruction) {
   if (instruction == 'l') {
      return load(adddress);
   } 
   if (instruction == 's') {
      return store(adddress);
   } else {
      return 0;
   }
}

int Cache::load(uint32_t address){
    // break address into components
    uint32_t tag = address;
    int offset_size = std::log2(bytes);
    uint32_t offset_and_val = (1<<offset_size)-1;
    uint32_t offset = tag & offset_and_val;
    tag = (tag >> offset_size); 
    int index_size = std::log2(blocks);
    uint32_t index_and_val = (1<<index_size)-1;
    uint32_t index = address & index_and_val;
    tag = (tag >> index_size); 
    //TODO: add private settings members to set and slot (like lru and stuff)
    Set target_set = cache[index];
    current_ts++;
    bool hit = target_set.is_hit(tag,offset,current_ts); 
    if (hit) {
         return 1; //valid hit
    } else{
         current_ts++;
         target_set.pull_mem(tag,index,offset,current_ts);
    }

    
}

int Cache::store(uint32_t adddress){
   return 0;
}