#pragma once

#include <SQLiteCpp/SQLiteCpp.h>

struct Page {
    const char* address;
    char* content;
    int code;
};

class Data {
    private:
        const char* m_filename;
    public:
        SQLite::Database* db;
        Data(const char* filename);
        int save_page(Page page);
};
