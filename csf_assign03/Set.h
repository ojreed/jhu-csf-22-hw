#ifndef SET_H
#define SET_H
#include "Slot.h"


class Set {
    public:
        Set(int blocks, int bytes, bool write_alloc, bool write_thr, bool lru); //default constructor
        bool is_hit(uint32_t tag, uint32_t offset); //checks if block exists in cache
        void pull_mem(uint32_t tag, uint32_t index, uint32_t offset);//used to pull memory from DRAM
        
        // get method
        std::vector<Slot> getSet() {
            return set;
        }

    private:
        int blocks;
        int bytes;
        int tag;
        bool write_alloc;
        bool write_thr;
        bool lru;
        std::vector<Slot> set; //vector of slots
};

#endif // SET_H