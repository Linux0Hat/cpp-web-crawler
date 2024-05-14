#include <iostream>
#include <string>
#include <curl/curl.h>

#include "network.h"
#include "colormod.h"

namespace net {
    size_t write_callback(void* ptr, size_t size, size_t nmemb, std::string* data) {
        data->append((char*)ptr, size * nmemb);
        return size * nmemb;
    }

    Page get_page(const char* address) {
        Page result = { address, nullptr, 0 };
        try {
            CURL* curl = curl_easy_init();
            if (curl) {
                curl_easy_setopt(curl, CURLOPT_URL, address);
                curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

                std::string response_data;
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

                CURLcode res = curl_easy_perform(curl);
                if (res == CURLE_OK) {
                    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &result.code);

                    result.content = new char[response_data.length() + 1];
                    strcpy(result.content, response_data.c_str());
                    std::cout << cyan << address << blue << " " << result.code << std::endl;
                } else {
                    std::cerr << red << "curl_easy_perform() failed: " << def << curl_easy_strerror(res) << std::endl;
                }

                curl_easy_cleanup(curl);
            } else {
                std::cerr << red <<"Failed to initialize curl" << def << std::endl;
            }
        }
        catch(error_t e) {}

        return result;
    }
}