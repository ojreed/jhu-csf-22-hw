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
};

cache_simulator::cache_simulator(int sets, int blocks, int bytes, bool write_alloc, bool write_thr, bool lru)
{
    int sets = sets;
    int blocks = blocks;
    int bytes = bytes;
    bool write_alloc = write_alloc;
    bool write_thr = write_thr;
    bool lru = lru;

}

cache_simulator::~cache_simulator()
{
}
