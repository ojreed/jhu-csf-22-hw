// Slot.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Slot.h"

// default constructor
Slot::Slot(int bytes, bool write_alloc, bool write_thr, bool lru)
{
   tag = NULL;
	valid = false;
   if (lru){
      load_ts = NULL;
      access_ts = 0;
   } else {
      load_ts = 0;
      access_ts = NULL;
   }
}
