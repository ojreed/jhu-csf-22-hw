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
   this->bytes = bytes;
   this->write_alloc = write_alloc;
   this->write_thr = write_thr;
   this->lru = lru;
   tag = 0;
   valid = false;
   diff_from_mem = true;
   this->ts = 0;
}
