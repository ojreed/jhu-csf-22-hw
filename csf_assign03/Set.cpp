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
   for (int x = 0; x<blocks;x++) {
      set.push_back(Slot(bytes,write_alloc,write_thr,lru));
   }
}