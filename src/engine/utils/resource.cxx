#include "resource.hxx"

#include <fstream>
#include <stdexcept>

#include "../utils/logger.hxx"

#include <Windows.h>

using namespace std::literals;

namespace compleks {

resource::resource()
{
}

resource::resource(std::string name, std::string type)
{
    logger::info("Loading app resource " + name);
    HRSRC res = FindResourceA(NULL, name.c_str(), type.c_str());
    if (!res) {
        throw std::runtime_error("Could not find resource " + name);
    }

    HGLOBAL res_ptr = LoadResource(NULL, res);
    if (!res_ptr) {
        throw std::runtime_error("Could not load resource " + name);
    }

    ptr = (char *)LockResource(res_ptr);
    if (!ptr) {
        throw std::runtime_error("Could not lock resource.");
    }

    size = SizeofResource(NULL, res);
    if (!size) {
        throw std::runtime_error("Could not get resource size.");
    }
}

resource::resource(std::string path)
    : path(path)
{
    reload();
}

resource::~resource()
{
    logger::info("Releasing resource");
}

void resource::reload()
{
    if (path.empty()) {
        return;
    }

    logger::info("Loading resource file " + path);
    std::ifstream fs(path, std::ios::ate | std::ios::binary);

    if (!fs) {
        throw std::runtime_error("Could not open resource "s + path);
    }

    auto end = fs.tellg();
    fs.seekg(0, std::ios::beg);
    size = (size_t)(end - fs.tellg());

    if (size == 0)
        return;

    uptr = std::make_unique<char[]>(size);
    ptr = uptr.get();

    if (!fs.read((char *)ptr, size)) {
        throw std::runtime_error("Could not read resource "s + path);
    }
}

}
