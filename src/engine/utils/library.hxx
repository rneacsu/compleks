#ifndef __COMPLEKS_UTILS_LIBRARY_HXX
#define __COMPLEKS_UTILS_LIBRARY_HXX

#include <map>
#include <utility>

#include "logger.hxx"

namespace compleks {

template <class T> class library {
public:
    template <class... Args> static void add(std::string id, Args&&... args)
    {
        remove(id);
        lib.emplace(std::piecewise_construct, std::forward_as_tuple(id),
            std::forward_as_tuple(args...));
    }

    static void remove(std::string id)
    {
        auto pos = lib.find(id);
        if (pos != lib.end()) {
            lib.erase(pos);
        }
    }

    static T& get(std::string id)
    {
        return lib.at(id);
    }

private:
    static std::map<std::string, T> lib;
};

template <class T> std::map<std::string, T> library<T>::lib;

} // namespace compleks

#endif // __COMPLEKS_UTILS_LIBRARY_HXX