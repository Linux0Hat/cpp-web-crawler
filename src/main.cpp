#include <iostream>
#include <regex>
#include <unistd.h>

#include "network.h"
#include "data.h"
#include "waitlist.h"
#include "colormod.h"
#include "asyncRunner.h"

int main(int argc, char** argv){
    std::cout << "Hello, from web-crawler!\n";
    Data db("database.db");
    Waitlist wl(&db);
    char* input;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--clear-waitlist") == 0) {
            std::cout << "Clearing the waitlist..." << std::endl;
            wl.clear();
        } else {
            std::cout << "URL provided: " << argv[i] << std::endl;
            input = argv[i];
        }
    }
    std::regex r(R"(^(?:(?:https?|ftp)://)(?:\S+@)?(?:(?!10(?:\.\d{1,3}){3})(?!127(?:\.\d{1,3}){3})(?!169\.254(?:\.\d{1,3}){2})(?!192\.168(?:\.\d{1,3}){2})(?!172\.(?:1[6-9]|2\d|3[0-1])(?:\.\d{1,3}){2})(?:[1-9]\d?|1\d\d|2[01]\d|22[0-3])(?:\.(?:1?\d{1,2}|2[0-4]\d|25[0-5])){2}(?:\.(?:[1-9]\d?|1\d\d|2[0-4]\d|25[0-4]))|(?:[a-z\u00a1-\uffff0-9]+-)*[a-z\u00a1-\uffff0-9]+(?:\.(?:[a-z\u00a1-\uffff0-9]+-)*[a-z\u00a1-\uffff0-9]+)*(?:\.(?:[a-z\u00a1-\uffff]{2,})))(?::\d{2,5})?(?:/\S*)?$)");
    if (!std::regex_match(input, r)) {
                std::cerr << red << "Bad URL." << def << std::endl;
                return 1;
            }
    AsyncRunner runner([&]() { wl.step(); }, 0, 3);

    runner.start();

    wl.add_page_to_waitlist(input);
    
    while(true) {
        sleep(1);
        if(wl.is_empty()) {
            sleep(2);
            if(wl.is_empty()) {
                std::cout << def << "Waitlist is empty, end." << std::endl;
                return 0;
            }
        }
    }
}