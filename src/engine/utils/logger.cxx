#include "logger.hxx"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace compleks {

void logger::log(level type, std::string msg)
{
    auto now = std::chrono::system_clock().now();
    auto time_t_now = std::chrono::system_clock().to_time_t(now);

    struct tm time_info;
    localtime_s(&time_info, &time_t_now);
    int miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                          now.time_since_epoch())
                          .count()
        % 1000;

    std::stringstream ss;
    ss << std::put_time(&time_info, "%H:%M:%S") << "." << std::setfill('0')
       << std::setw(3) << miliseconds;

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

    std::cerr << "\x1B[" << color_code << "m[" << ss.str() << "] " << msg
              << "\x1B[0m\n";
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
