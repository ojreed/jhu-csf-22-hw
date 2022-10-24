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
// Cache::Cache(int sets, int blocks, int bytes, bool write_alloc, bool write_thr, bool lru, uint32_t* cache_ctr_in,uint32_t* mem_ctr_in)
// {
//    this->sets = sets;
//    this->blocks = blocks;
//    this->bytes = bytes;
//    this->write_alloc = write_alloc;
//    this->write_thr = write_thr;
//    this->lru = lru;
//    this->tag = tag;
//    this->current_ts = 0;
//    this->cache_ctr = cache_ctr_in;
//    this->mem_ctr = mem_ctr_in;
//    for (int x = 0; x < sets; x++)
//    {
//       cache.push_back(Set(blocks, bytes, write_alloc, write_thr, lru, cache_ctr, mem_ctr));
//    }
// }


int Cache::split_address(uint32_t address, uint32_t* tag, uint32_t* index, uint32_t* offset) {
   //TODO: store log2 of bytes and blocks in the cache --> speed up
   //store the address in the tag (will be witteled down)
   *tag = address;
   int offset_size = std::log2(bytes);//ofset size is the number of bits needed to represent offset 
   uint32_t one = 1; //useful value
   uint32_t offset_and_val = (one << offset_size) - 1; //what we need to bitwise and the address by (n 1's)
   *offset = *tag & offset_and_val; //offset is a portion of the address (in tag)
   *tag = (*tag >> offset_size); //wittle down tag (get rid of what we already snipped off)
   int index_size = std::log2(sets); //index size is the number of bits needed to represent index 
   uint32_t index_and_val = (one << index_size) - 1; //what we need to bitwise and the address by (n 1's)
   *index = *tag & index_and_val; //index is a portion of the address (in tag)
   *tag = (*tag >> index_size);//whittle down tag --> tag is what is left over
   return 0; //results passed out with use of pointers
}

// helper function to make calling of load and store directly from trace simpler
int Cache::access(uint32_t adddress, char instruction)
{
   if (instruction == 'l')
   {
      return load(adddress); //will handle the whole load proccess
   }
   if (instruction == 's')
   {
      return store(adddress); //will handle the whole write proccess
   }
   else
   {
      return -1; //invalid operation
   }
}

int Cache::load(uint32_t address)
{
   // break address into components
   uint32_t tag = 0;
   uint32_t index = 0;
   uint32_t offset = 0;
   Cache::split_address(address,&tag,&index,&offset);
   Set *target_set = &(cache[index]); //get the target set from the address index
   current_ts++; //increment timestamp to keep time
   bool hit = (*target_set).is_hit(tag, offset, current_ts); //look to see if the tag exists within the correct set (by index)
   if (hit) //this means the block exists
   {
      (*cache_ctr)++; //increment the number of accesses to cache
      return 1; // valid hit
   }
   else //block does not exist
   {
      // current_ts++;
      (*target_set).pull_mem(tag, index, offset, current_ts); //find the oldest element (by mode) and load value from DRAM to that block
      return 0;
   }
}

int Cache::store(uint32_t address)
{
   // break address into components
   uint32_t tag = 0;
   uint32_t index = 0;
   uint32_t offset = 0;
   Cache::split_address(address,&tag,&index,&offset);
   Set *target_set = &(cache[index]); //get the target set from the address index
   current_ts++; //increment timestamp to keep time
   bool hit = (*target_set).is_hit(tag, offset, current_ts); //look to see if the tag exists within the correct set (by index)
   if (hit)
   {
      if (this->write_thr) //write through (Update Cache and access memory)
      { // write to memory immediately
        // write to cache
        (*cache_ctr)++; //increment the number of accesses to cache
        // write to mem
        (*mem_ctr)++; //increment the number of accesses to mem
      }
      else //write back (dont modify memory until overwrite)
      {
         // write to cache
         (*cache_ctr)++; //increment the number of accesses to cache
         // do not write to mem --> defer to replacment
        Slot *slot = (*target_set).get_slot(tag, offset,current_ts);
        (*slot).set_diff_from_mem(true);
      }
      return 1; // valid hit
   }
   else
   {
      current_ts++;
      if (this->write_alloc) //write alloc (pulls from mem)
      {
         // write information from DRAM into cache
         (*target_set).pull_mem(tag, index, offset, current_ts);
         //write data to cache
         (*cache_ctr)++; //increment the number of accesses to cache
         (*miss_mem_ctr)++
      }
      else //No Write Alloc (doesnt bother to pull mem)
      {
         // writes straight to memory
         (*miss_mem_ctr)++; //increment the number of accesses to mem
         // no cache call
      }
      return 0;
   }
}