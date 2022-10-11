#ifndef CACHE_SIMULATOR_H
#define CACHE_SIMULATOR_H

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

public:
    cache_simulator(int sets, int blocks, int bytes, bool write_alloc, bool write_thr, bool lru);
    ~cache_simulator();
    void printResult(std::vector<int> values);
    std::vector<int> parseTraces();

};

cache_simulator::cache_simulator(int sets, int blocks, int bytes, bool write_alloc, bool write_thr, bool lru)
{
    this->sets = sets;
    this->blocks = blocks;
    this->bytes = bytes;
    this->write_alloc = write_alloc;
    this->write_thr = write_thr;
    this->lru = lru;

}

cache_simulator::~cache_simulator()
{
}



#endif // CACHE_SIMULATOR_H
