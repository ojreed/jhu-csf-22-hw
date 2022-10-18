#ifndef SLOT_H
#define SLOT_H

class Slot
{

public:
    Slot(int bytes, bool write_alloc, bool write_thr, bool lru); // default constructor

    // "get" methods
    uint32_t getTag()
    {
        return tag;
    }

    bool getValid()
    {
        return valid;
    }

    uint32_t getTS()
    {
        return this->ts;
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

    void setTS(uint32_t new_access)
    {
        this->ts = new_access;
    }

private:
    uint32_t tag;
    bool valid;
    uint32_t ts;
    int bytes;
    bool write_alloc;
    bool write_thr;
    bool lru;
};

#endif // SLOT_H