// Cache.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include "Cache.h"
#include "Set.h"

int Cache::split_address(uint32_t address, uint32_t *tag, uint32_t *index, uint32_t *offset)
{
   // store the address in the tag (will be witteled down)
   *tag = address;
   int offset_size = std::log2(bytes); // ofset size is the number of bits needed to represent offset
   uint32_t one = 1; // useful value
   uint32_t offset_and_val = (one << offset_size) - 1; // what we need to bitwise and the address by (n 1's)
   *offset = *tag & offset_and_val; // offset is a portion of the address (in tag)
   *tag = (*tag >> offset_size); // wittle down tag (get rid of what we already snipped off)
   int index_size = std::log2(sets); // index size is the number of bits needed to represent index
   uint32_t index_and_val = (one << index_size) - 1; // what we need to bitwise and the address by (n 1's)
   *index = *tag & index_and_val; // index is a portion of the address (in tag)
   *tag = (*tag >> index_size); // whittle down tag --> tag is what is left over
   return 0; // results passed out with use of pointers
}

// helper function to make calling of load and store directly from trace simpler
int Cache::access(uint32_t adddress, char instruction)
{
   if (instruction == 'l')
   {
      return load(adddress); // will handle the whole load proccess
   }
   if (instruction == 's')
   {
      return store(adddress); // will handle the whole write proccess
   }
   else
   {
      return -1; // invalid operation
   }
}

int Cache::load(uint32_t address)
{
   // break address into components
   uint32_t tag = 0;
   uint32_t index = 0;
   uint32_t offset = 0;
   Cache::split_address(address, &tag, &index, &offset);
   Set *target_set = &(cache[index]); // get the target set from the address index
   current_ts++; // increment timestamp to keep time
   bool hit = (*target_set).is_hit(tag, current_ts); // look to see if the tag exists within the correct set (by index)
   if (hit) // this means the block exists
   {
      (*cache_ctr)+=1; // increment the number of accesses to cache
      return 1; // valid hit
   }
   else // block does not exist
   {
      //cycels handeled by pull_mem 
      //pulls from mem wired into cache
      (*target_set).pull_mem(tag, current_ts); // find the oldest element (by mode) and load value from DRAM to that block
      return 0; 
   }
}

int Cache::store(uint32_t address)
{
   // break address into components
   uint32_t tag = 0;
   uint32_t index = 0;
   uint32_t offset = 0;
   Cache::split_address(address, &tag, &index, &offset);
   Set *target_set = &(cache[index]); // get the target set from the address index
   current_ts++; // increment timestamp to keep time
   bool hit = (*target_set).is_hit(tag, current_ts); // look to see if the tag exists within the correct set (by index)
   if (hit)
   {
      if (this->write_thr) // write through (Update Cache and access memory)
      {  
         (*cache_ctr)+=100; // increment the number of accesses to cache
      }
      else // write back (dont modify memory until overwrite)
      {
         (*cache_ctr)+=1; // write to cache and mark dirty
         Slot *slot = (*target_set).get_slot(tag, current_ts);
         (*slot).set_diff_from_mem(true); // set dirty bit
      }
      return 1; // valid hit
   }
   else
   {
      current_ts++;
      if (this->write_alloc) // write alloc (pulls from mem)
      {
         // write information from DRAM into cache
         //handels cycles counts - writes to mem - wired into cache
         (*target_set).pull_mem(tag, current_ts);
      }
      else // No Write Alloc (doesnt bother to pull mem, writes straight to memory)
      {
         (*cache_ctr)+=100; // increment the number of accesses to cache
         // no cache call
      }
      return 0;
   }
}