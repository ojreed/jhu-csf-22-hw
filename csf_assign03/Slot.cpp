// Slot.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Slot.h"

// default constructor
Slot::Slot(uint32_t bytes, bool write_alloc, bool write_thr, bool lru)
{
   // cache parameters
   this->bytes = bytes;
   this->write_alloc = write_alloc;
   this->write_thr = write_thr;
   this->lru = lru;
   // store tag
   tag = 0;
   valid = false; // by defualt block is invalid
   diff_from_mem = true; // by defualt diff from memory
   ts = 0; // by defualt no timestamp
}
