#ifndef __COMPLEKS_UTILS_RESOURCE_HXX
#define __COMPLEKS_UTILS_RESOURCE_HXX

#include <memory>
#include <string>

namespace compleks {

class resource {
public:
    char *ptr;
    size_t size;

    resource(std::string name, std::string type);
    resource(std::string path);
    resource();
    ~resource();

    void reload();

private:
    std::unique_ptr<char[]> uptr;
    std::string path;
};

}

#endif // __COMPLEKS_UTILS_RESOURCE_HXX
