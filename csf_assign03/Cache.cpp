// Cache.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Cache.h"
#include "Set.h"

// default constructor
Cache::Cache(int sets, int blocks, int bytes, bool write_alloc, bool write_thr, bool lru)
{
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

int Cache::load(uint32_t adddress);
int Cache::store(uint32_t adddress);