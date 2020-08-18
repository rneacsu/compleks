#include "logger.hxx"

#include <iostream>
#include <ctime>

namespace compleks
{

void logger::log(level type, std::string msg)
{
    time_t raw_time;
    struct tm time_info;
    char buf[80];

    time(&raw_time);
    localtime_s(&time_info, &raw_time);

    strftime(buf, 80, "%Y-%m-%d %H:%M:%S", &time_info);


    int color_code;
    switch (type) {
    case WARN:
        color_code = 33;
        break;
    case ERROR:
        color_code = 31;
        break;
    default:
        color_code = 37;
    }

    std::cerr << "\x1B[" << color_code << "m[" << buf << "] " << msg << "\x1B[0m\n";
}

void logger::info(std::string msg)
{
    log(INFO, msg);
}


void logger::warn(std::string msg)
{
    log(WARN, msg);
}


void logger::error(std::string msg)
{
    log(ERROR, msg);
}

}