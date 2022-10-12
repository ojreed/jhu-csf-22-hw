#ifndef CACHE_H
#define CACHE_H
#include "Set.h"


class Cache {

    public:
        Cache(int sets, int blocks, int bytes, bool write_alloc, bool write_thr, bool lru); //default constructor

        std::vector<Set> getCache() {
            return cache;
        }

    private:
        //stuff
        std::vector<Set> cache; //vector of sets
};

#endif // CACHE_H