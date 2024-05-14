#include <iostream>
#include <vector>

#include "waitlist.h"
#include "data.h"
#include "network.h"
#include "colormod.h"
#include "parser.h"

Waitlist::Waitlist(Data* db) {
    m_db = db;
}

const char* Waitlist::pop_page_in_waitlist() {
    try {
        char get_pages[] = "SELECT id, address FROM waitlist";
        SQLite::Statement query(*m_db->db, get_pages);
        if(query.executeStep()) {
            int id = query.getColumn(0);
            const char* address = query.getColumn(1);
            char* address_ = strdup(address);
            char del_page[] = "DELETE FROM waitlist WHERE id=:id";
            SQLite::Statement query_(*m_db->db, del_page);
            query_.bind(":id", id);
            query_.exec();
            return address_;
        }

        return NULL;
    } 
    
    catch(error_t e){
        std::cerr << red << "Error while load an element of waitlist :\n" << def << e << std::endl;
        return NULL;
    }
}

int Waitlist::step() {
    const char*  address = pop_page_in_waitlist();
    if(address) {
        std::cout << magenta << "From waitlist : " << def;
        Page page = net::get_page(address);
        if(page.content) {
            std::vector<std::string> vector = parser::scan(page);
            for(int i; i<vector.size(); i++) {
                add_page_to_waitlist(vector[i].c_str());
            }
            std::cout << magenta << "From waitlist : " << def;
            m_db->save_page(page);
        }
    }
}

int Waitlist::add_page_to_waitlist(const char* address) {
    try {
        char check_page[] = "SELECT id FROM waitlist WHERE address=:address";
        SQLite::Statement query(*m_db->db, check_page);
        query.bind(":address", address);
        if(query.executeStep()) {
            return 0;
            
        }

        char save_page[] = "INSERT INTO waitlist ( address) VALUES( :address );";
        SQLite::Statement query_(*m_db->db, save_page);
        query_.bind(":address", address);
        query_.exec();
        std::cout << cyan << address << green << " added to waitlist." << def << std::endl;
        return 0;
    } 
    
    catch(error_t e){
        std::cerr << red << "Error while add " << cyan << address << red << " to waitlist :\n" << def << e << std::endl;
        return 1;
    }
}

int Waitlist::clear() {
    try {
        char clear_pages[] = "DELETE FROM waitlist;";
        SQLite::Statement query(*m_db->db, clear_pages);
        query.exec();
        std::cout << green << "Waitlist clear." << def << std::endl;
    }
    catch(error_t e) {
        std::cerr << red << "Error while clear waitlist :\n" << def << e << std::endl;
        return 1;
    }
}

bool Waitlist::is_empty() {
    char get_pages[] = "SELECT * FROM waitlist";
    SQLite::Statement query(*m_db->db, get_pages);
    if(query.executeStep()) {
        return false;
    }
    return true;
}