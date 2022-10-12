#ifndef SLOT_H
#define SLOT_H


class Slot {

    public:
        Slot(int bytes, bool write_alloc, bool write_thr, bool lru); //default constructor

        // "get" methods
        uint32_t getTag(){
            return tag;
        }

		bool getValid(){
            return valid;
        }

		uint32_t getLoadTs(){
            return load_ts;
        }

        uint32_t getAccessTs(){
            return access_ts;
        }

    private:
        uint32_t tag;
		bool valid;
		uint32_t load_ts;
        uint32_t access_ts;
};

#endif // SLOT_H