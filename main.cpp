#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include "gps.h"
#include <iostream>
using namespace std;

size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

int main() {
    std::string url = "http://192.168.1.133:5001/gps";
    // std::string url = "http://192.168.1.133";
    std::string device = "/dev/tty.usbmodem101";
    Gps data(device, url, 57600);
    data.readLine();


    // return 0;
    // curl_global_init(CURL_GLOBAL_DEFAULT);
    // auto curl = curl_easy_init();
    // if (curl) {
    //     curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    //     curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    //     curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
    //     curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

    //     std::string response_string;
    //     std::string header_string;
    //     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
    //     curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
    //     curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

    //     curl_easy_perform(curl);
    //     cout << response_string;
    //     curl_easy_cleanup(curl);
    //     curl_global_cleanup();
    //     curl = NULL;
    // }
    
}
