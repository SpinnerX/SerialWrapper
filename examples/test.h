#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <curl/curl.h>
#include <cmath>
#include <string>
#include <memory>

struct GpsData{
    double longitude = 0.f;
    double latitude = 0.f;
};

class gpsRead {
    boost::asio::io_service ios;
    boost::asio::serial_port sp;

public:
    gpsRead(std::string port, unsigned int baud_rate) : ios(), sp(ios, baud_rate) {
        sp.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    }

    void send_request(GpsData data, std::string url) 
    {
        // Here, you can send a HTTP POST request to the specified URL with the longitude/latitude.
        // This is usually done using libraries like libcurl. For simplicity, we do not
        // implement the actual POST request in this code snippet.
    }

    GpsData get_position(std::string url = "") 
    {
        GpsData _data;
        try {
            std::string gps_serial;
            boost::asio::streambuf read_buf;
            boost::asio::read_until(sp, read_buf, "\n");
            std::istream is(&read_buf);
            std::getline(is, gps_serial);

            size_t start = gps_serial.find("$GNRMC");
            if (start != std::string::npos) {
                gps_serial = gps_serial.substr(start);
                std::replace(gps_serial.begin(), gps_serial.end(), ',', ' ');
                std::istringstream iss(gps_serial);
                std::vector<std::string> parts(
                    (std::istream_iterator<std::string>(iss)), 
                     std::istream_iterator<std::string>());

                if (parts[2] == "A") {
                    double latitude = std::stod(parts[3].substr(0, 2)) +
                                      std::stod(parts[3].substr(2)) / 60;
                    if (parts[4] == "S") {
                        latitude *= -1;
                    }
                    double longitude = std::stod(parts[5].substr(0, 3)) +
                                       std::stod(parts[5].substr(3)) / 60;
                    if (parts[6] == "W") {
                        longitude *= -1;
                    }
                    // _data = {longitude, latitude};
                    _data.longitude = longitude;
                    _data.latitude = latitude;
                    if (url != "") {
                        send_request(_data, url);
                    }
                }
            }
        } catch (...) {
            // _data = {std::nan("error"), std::nan("error")};
            _data.longitude = std::nan("error");
            _data.latitude = std::nan("error");
        }
        return _data;
    }
};