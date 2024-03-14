#pragma once
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <curl/curl.h>
#include <cmath>
#include <string>
#include <memory>
#include <cmath> // std::is_nan

// Coordinates contain the longitude and latitude
struct GpsCoordinates{
    double longitude = 0.f;
    double latitude = 0.f;
    
    bool isCoordinatesValid() const {
        return std::isnan(longitude) and std::isnan(latitude);
    }
};

class Gps{
public:
    /**
     * Constructor.
     * \param port device name, example "/dev/ttyUSB0" or "COM4"
     * \param baud_rate communication speed, example 9600 or 115200
     * \throws boost::system::system_error if cannot open the
     * serial device
     */
    Gps(const std::string& port, const std::string& url, unsigned int baud_rate) : io(), serial(io,port), url(url){
        serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    }

    /**
     * Write a string to the serial device.
     * \param s string to write
     * \throws boost::system::system_error on failure
     */
    void writeString(std::string s){
        boost::asio::write(serial,boost::asio::buffer(s.c_str(),s.size()));
    }

    void getPosition(){
        CURL* curl = curl_easy_init();
        CURLcode res;
        GpsCoordinates coords;
        std::string testBuffer = "";

        if(!curl){
            return;
        }

        curl_easy_setopt(curl, CURLOPT_URL, &url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Gps::WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &testBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    /**
     * Blocks until a line is received from the serial device.
     * Eventual '\n' or '\r\n' characters at the end of the string are removed.
     * \return a string containing the received line
     * \throws boost::system::system_error on failure
     */
    std::string readLine(){
        //Reading data char by char, code is optimized for simplicity, not speed
        using namespace boost;
        char c;
        std::string result;
        while(true){
            asio::read(serial, asio::buffer(&c,1));
            // result += c;
            std::cout << c;
            // std::cout << c;
            // switch(c){
            //     case '\r':
            //         break;
            //     case '\n':
            //         return result;
            //     default:
            //         result+=c;
            // }
        }
        return result;
    }
private:
    // Checking if the position of longitu
    bool position_is_error(GpsCoordinates position) {
        return std::isnan(position.latitude) || std::isnan(position.longitude);
    }

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

private:
    boost::asio::io_service io;
    boost::asio::serial_port serial;
    std::string url;
};