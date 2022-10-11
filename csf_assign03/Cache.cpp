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