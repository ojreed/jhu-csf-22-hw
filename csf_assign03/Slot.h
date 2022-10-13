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

        // "set" methods
        uint32_t setTag(uint32_t new_tag){
            tag = new_tag;
        }

		bool setValid(bool new_valid){
            valid = new_valid;
        }

		uint32_t setLoadTs(uint32_t new_load){
            load_ts = new_load;
        }

        uint32_t setAccessTs(uint32_t new_access){
            access_ts = new_access;
        }


    private:
        uint32_t tag;
		bool valid;
		uint32_t load_ts;
        uint32_t access_ts;
};

#endif // SLOT_H