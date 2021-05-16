#ifndef __COMPLEKS_UTILS_LOGGER_HXX
#define __COMPLEKS_UTILS_LOGGER_HXX

#include <mutex>
#include <string>

namespace compleks {

class logger {
public:
    static void info(std::string msg);
    static void warn(std::string msg);
    static void error(std::string msg);

private:
    enum level { INFO, WARN, ERROR, DEBUG };

    logger() = delete;
    static void log(level type, std::string msg);
    static std::mutex lock;
};

} // namespace compleks

#endif // __COMPLEKS_UTILS_LOGGER_HXX
