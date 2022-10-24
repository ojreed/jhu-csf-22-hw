#ifndef SLOT_H
#define SLOT_H

class Slot
{

public:
    Slot(uint32_t bytes, bool write_alloc, bool write_thr, bool lru); // default constructor

    // "get" methods
    uint32_t getTag()
    {
        return tag;
    }

    bool getValid()
    {
        return valid;
    }

    uint64_t getTS()
    {
        return this->ts;
    }

    bool is_valid()
    {
        return this->valid;
    }

    bool is_diff_from_mem()
    {
        return this->diff_from_mem;
    }

    // "set" methods
    void setTag(uint32_t new_tag)
    {
        this->tag = new_tag;
    }

    void setValid(bool new_valid)
    {
        this->valid = new_valid;
    }

    void setTS(uint64_t new_access)
    {
        this->ts = new_access;
    }

    void set_valid(bool truth) {
        this->valid = truth;
    }

    void set_diff_from_mem(bool truth) {
        this->diff_from_mem = truth;
    }

private:
    uint32_t tag;
    bool valid;
    uint64_t ts;
    uint32_t bytes;
    bool write_alloc;
    bool write_thr;
    bool lru;
    bool diff_from_mem;
};

#endif // SLOT_H