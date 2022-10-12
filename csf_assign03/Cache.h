#ifndef CACHE_H
#define CACHE_H
#include "Set.h"


class Cache {

    public:
        Cache(int sets, int blocks, int bytes, bool write_alloc, bool write_thr, bool lru); //default constructor
        int access(uint32_t adddress,char instruction);
        int load(uint32_t adddress);
        int store(uint32_t adddress);
        //more stuff

    private:
        //stuff
        std::vector<Set> cache; //vector of sets
};

#endif // CACHE_H