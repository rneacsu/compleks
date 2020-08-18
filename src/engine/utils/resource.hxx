#ifndef __COMPLEKS_UTILS_RESOURCE_HXX
#define __COMPLEKS_UTILS_RESOURCE_HXX

#include <memory>
#include <string>

namespace compleks {

class resource {
public:
    char *ptr;
    std::unique_ptr<char[]> uptr;
    size_t size;

    resource(char const *name, char const *type);
    resource(char const *path);
    resource();
    ~resource();

    void reload();

private:
    std::string path;
};

}

#endif // __COMPLEKS_UTILS_RESOURCE_HXX