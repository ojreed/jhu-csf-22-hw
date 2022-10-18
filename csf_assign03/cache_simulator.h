#ifndef CACHE_SIMULATOR_H
#define CACHE_SIMULATOR_H
#include "Cache.h"
#include <vector>

class cache_simulator
{
private:
    /* data */
    int sets;
    int blocks;
    int bytes;
    bool write_alloc;
    bool write_thr;
    bool lru;
    Cache cache;

public:
    cache_simulator();
    cache_simulator(int sets, int blocks, int bytes, bool write_alloc, bool write_thr, bool lru) {
        this->sets = sets;
        this->blocks = blocks;
        this->bytes = bytes;
        this->write_alloc = write_alloc;
        this->write_thr = write_thr;
        this->lru = lru;
        cache = Cache(sets,blocks,bytes,write_alloc,write_thr,lru);
    }
    ~cache_simulator();
    void printResult(std::vector<int> values);
    std::vector<int> parseTraces();
    

};





#endif // CACHE_SIMULATOR_H
