#include <exception>
#include <string>

#include <compleks.h>

#include "demo.hxx"

int main(int, char *[])
{
    compleks::logger::info("Starting...");

    int rc = EXIT_SUCCESS;
    try {
        demo win;
        win.run();
    } catch (std::exception &e) {
        compleks::logger::error(std::string("Exception occured: ") + e.what());
        rc = EXIT_FAILURE;
    }

    compleks::logger::info("Exiting...");

    return rc;
}
