#ifndef CACHE_H
#define CACHE_H
#include "Set.h"


class Cache {

    public:
        Cache(int sets, int blocks, int bytes, bool write_alloc, bool write_thr, bool lru); //default constructor
        int access(uint32_t adddress,char instruction);
        int load(uint32_t adddress);
        int store(uint32_t adddress);
        uint32_t current_ts;
        //more stuff

        std::vector<Set> getCache() {
            return cache;
        }

    private:
        //stuff
        std::vector<Set> cache; //vector of sets
        int sets;
        int blocks;
        int bytes;
        int tag;
        bool write_alloc;
        bool write_thr;
        bool lru;
};

#endif // CACHE_H