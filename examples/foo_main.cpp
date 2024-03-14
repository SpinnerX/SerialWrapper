#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> // To include O_RDWR, O_NOCTTY
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <array>
#include <istream>
#include <stdlib.h>
#include <unordered_map>
#include "requests.h"
#include <iostream>
#include <fstream>
using namespace std;

struct GpsData{
    int longitutde=0, latitude=0;
};

struct Gps{


};

namespace std{
    template<typename _Tp, size_t size>
    std::ostream& operator<<(std::ostream& outs, const array<_Tp, size> array){
        for(size_t i = 0; i < array.size(); i++) outs << array[i];
        return outs;
    }
};

int main(int argc, char *argv[]){
    std::string deviceName = "/dev/tty.usbmodem101";
    std::array<char, 256> buf;
    int n;
    int i;
    int count = 0;
    int baudrate = B9600;
    struct termios toptions;

    int fd = open(deviceName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd == -1) {
        printf("Device %s, could not be connected! Try again.\n", deviceName.c_str());
        return -1;
    }

    if (tcgetattr(fd, &toptions) < 0) {
        printf("serialport_init: Couldn't get term attributes");
        return -1;
    }

    speed_t brate = baudrate; // let you override switch below if needed
    switch(baudrate) {
        case 4800: brate=B4800;   break;
        case 9600: brate=B9600;   break;
        #if defined B14400
            case 14400: brate=B14400;  break;
        #endif
            case 19200: brate=B19200;  break;
        #if defined B28800
            case 28800: brate=B28800;  break;
        #endif
        case 38400: brate=B38400;  break;
        case 57600: brate=B57600;  break;
        case 115200: brate=B115200; break;
    }
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);

    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    // no flow control
    toptions.c_cflag &= ~CRTSCTS;

    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw

    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 20;

    if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
        perror("init_serialport: Couldn't set term attributes");
        return -1;
    }

    while (true) {
        n = read(fd, buf.data(), buf.size());
        if (n > 0) {
            buf[n] = 0;
            // printf("read %i bytes: %s", n, buf.data());
            // printf("reading %i bytes: ");
            std::cout << "Reading " << n << " bytes: " << buf;
            // for(size_t i = 0; i < buf.size(); i++) printf("%s", buf[0]);
        }
        // if (count == 0) {
        //     // n = write(fd, "Hello!", 6);
        //     // if (n < 0) {
        //     //     perror("Write failed");
        //     // }
        //     count++;
        // }
        sleep(1);
    }

    // try{
    //     http::Request request{"http://192.168.1.133:5001/gps"};
    //     const string body = "foo=1&bar=baz";
    //     const auto response = request.send("POST", body, {
    //         {"Longitude", "Latitude"}
    //     });
    //     std::cout << std::string{response.body.begin(), response.body.end()} << '\n'; // print the result

    // }
    // catch(const std::exception& e){
    //     std::cerr << "Request failed, error: " << e.what() << '\n';
    // }
}


// int main(){
//     bool serialRunning = true;
//     const int BAUDRATE = B38400; // Setting the bps rate. Could also use cfsetispeed and cfsetospeed
//     const std::string device = "/dev/tty.usbmodem101";
//     const int posix_src = 1;

//     int fd = open(device.c_str(), O_RDWR | O_NOCTTY );

//     struct termios oldtio, newtio;
    
//     if (fd < 0) {
//         printf("%s not connected\n", device.c_str());
//         exit(-1);
//     }
//     printf("%s is connected!\n", device.c_str());

//     tcgetattr(fd, &oldtio);
//     bzero(&newtio, sizeof(newtio));

//     newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
//     newtio.c_iflag = IGNPAR | ICRNL;
//     newtio.c_oflag = 0;

//     newtio.c_lflag = ICANON;

//     newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */ 
//     newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
//     newtio.c_cc[VERASE]   = 0;     /* del */
//     newtio.c_cc[VKILL]    = 0;     /* @ */
//     newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
//     newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
//     newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
//     // newtio.c_cc[VSWTC]    = 0;     /* '\0' */
//     newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */ 
//     newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
//     newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
//     newtio.c_cc[VEOL]     = 0;     /* '\0' */
//     newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
//     newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
//     newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
//     newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
//     newtio.c_cc[VEOL2]    = 0;     /* '\0' */

//     tcflush(fd, TCIFLUSH);
//     tcsetattr(fd, TCSANOW, &newtio);
//     std::array<char, 255> buffer;

//     while (serialRunning) {     /* loop until we have a terminating condition */
//         /* read blocks program execution until a line terminating character is 
//         input, even if more than 255 chars are input. If the number
//         of characters read is smaller than the number of chars available,
//         subsequent reads will return the remaining chars. res will be set
//         to the actual number of characters actually read */
//         int res = read(fd, buffer.data(), buffer.size());
//         buffer[res] = 0;             /* set end of string, so we can printf */
//         printf(":%s:%d\n", buffer.data(), res);
//         if (buffer[0]=='z') {
//             serialRunning = false;
//         }
//     }

// }