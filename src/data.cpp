#include <fstream>
#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>
#include <time.h>

#include "data.h"
#include "colormod.h"

#define time_limit 60


Data::Data(const char* filename) {
    db = new SQLite::Database(filename, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

    const char* create_pages_table = "CREATE TABLE IF NOT EXISTS "
                                     "pages (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                     "address TEXT, content BLOB,"
                                     "code INTEGER, datetime INTEGER)";

    const char* create_waitlist_table = "CREATE TABLE IF NOT EXISTS "
                                        "waitlist (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                        "address TEXT)";

    try {
        SQLite::Statement query_pages(*db, create_pages_table);
        query_pages.exec();

        SQLite::Statement query_waitlist(*db, create_waitlist_table);
        query_waitlist.exec();
    } catch (std::exception& e) {
        std::cerr << "SQLite error: " << e.what() << std::endl;
    }
}

int Data::save_page(Page page) {
    try {
        time_t rawtime;
        time(&rawtime);

        char check_page[] = "SELECT id, datetime FROM pages WHERE address=:address";
        SQLite::Statement query(*db, check_page);
        query.bind(":address", page.address);
        while(query.executeStep()) {
            int id = query.getColumn(0);
            int datetime = query.getColumn(1);
            if(datetime+time_limit > rawtime) {
                std::cout << cyan << page.address << yellow << " already exist." << def << std::endl;
                return 0;
            }
            
        }

        char save_page[] = "INSERT INTO pages ( address, content, code, datetime) VALUES( :address, :content, :code, :datetime );";
        SQLite::Statement query_(*db, save_page);
        query_.bind(":address", page.address);
        query_.bind(":content", page.content, strlen(page.content));
        query_.bind(":code", page.code);
        query_.bind(":datetime", rawtime);
        query_.exec();
        std::cout << cyan << page.address << green << " saved." << def << std::endl;
        return 0;
    } 
    
    catch(error_t e){
        std::cerr << red << "Error while saving " << cyan << page.address << red << " :\n" << def << e << std::endl;
        return 1;
    }
}