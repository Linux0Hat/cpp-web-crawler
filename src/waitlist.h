#pragma once

#include "data.h"

class Waitlist {
    private:
        Data* m_db;
    public:
        Waitlist(Data* db);
        int step();
        int add_page_to_waitlist(const char* address);
        const char* pop_page_in_waitlist();
        int clear();
        bool is_empty();
};