#ifndef __COMPLEKS_UTILS_LIBRARY_HXX
#define __COMPLEKS_UTILS_LIBRARY_HXX

#include <map>
#include <memory>
#include <string>
#include <utility>

namespace compleks {

template <class T> class library {
public:
    template <class... Args> void add(std::string id, Args &&...args)
    {
        remove(id);
        lib.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(id),
            std::forward_as_tuple(args...));
    }

    void remove(std::string id)
    {
        auto pos = lib.find(id);
        if (pos != lib.end()) {
            lib.erase(pos);
        }
    }

    T &get(std::string id)
    {
        return lib.at(id);
    }

    bool exists(std::string id)
    {
        return lib.find(id) != lib.end();
    }

private:
    std::map<std::string, T> lib;
};

} // namespace compleks

#endif // __COMPLEKS_UTILS_LIBRARY_HXX
