#ifndef SLOT_H
#define SLOT_H


class Slot {

    public:
        Slot(int bytes, bool write_alloc, bool write_thr, bool lru); //default constructor
        //more stuff

    private:
        uint32_t tag;
		bool valid;
		uint32_t load_ts;
        uint32_t access_ts;
};

#endif // SLOT_H