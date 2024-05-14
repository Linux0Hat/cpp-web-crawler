#include <string>
#include <iostream>
#include <regex>

#include "parser.h"
#include "data.h"

namespace parser {
    std::vector<std::string> scan(Page page) {
        static const std::regex hl_regex( "href=\"(.*?)\"", std::regex_constants::icase );
        const std::string text = page.content;

        std::vector<std::string> lst;
        std::copy(std::sregex_token_iterator(text.begin(), text.end(), hl_regex, 1),
                  std::sregex_token_iterator(),
                  std::back_inserter(lst));

        std::regex r(R"(^(?:(?:https?|ftp)://)(?:\S+@)?(?:(?!10(?:\.\d{1,3}){3})(?!127(?:\.\d{1,3}){3})(?!169\.254(?:\.\d{1,3}){2})(?!192\.168(?:\.\d{1,3}){2})(?!172\.(?:1[6-9]|2\d|3[0-1])(?:\.\d{1,3}){2})(?:[1-9]\d?|1\d\d|2[01]\d|22[0-3])(?:\.(?:1?\d{1,2}|2[0-4]\d|25[0-5])){2}(?:\.(?:[1-9]\d?|1\d\d|2[0-4]\d|25[0-4]))|(?:[a-z\u00a1-\uffff0-9]+-)*[a-z\u00a1-\uffff0-9]+(?:\.(?:[a-z\u00a1-\uffff0-9]+-)*[a-z\u00a1-\uffff0-9]+)*(?:\.(?:[a-z\u00a1-\uffff]{2,})))(?::\d{2,5})?(?:/\S*)?$)");
        auto removeCondition = [&r](const std::string& s) {
            return !std::regex_match(s, r);
        };

        lst.erase(std::remove_if(lst.begin(), lst.end(), removeCondition), lst.end());
            return lst;
    }
} 
