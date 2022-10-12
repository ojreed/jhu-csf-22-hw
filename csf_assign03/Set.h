#ifndef SET_H
#define SET_H
#include "Slot.h"


class Set {
    public:
        Set(int blocks, int bytes, bool write_alloc, bool write_thr, bool lru); //default constructor
        
        // get method
        std::vector<Slot> getSet() {
            return set;
        }

    private:
        std::vector<Slot> set; //vector of slots
};

#endif // SET_H