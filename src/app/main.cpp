#include <exception>
#include <string>

#include <compleks.h>

#include "demo_world.hxx"
#include "resource.h"

using namespace std::literals;

int main(int, char *[])
{
    compleks::logger::info("Starting...");

    try {
        demo_world win;

        win.set_icon(APP_ICON);
        win.run();

    } catch (std::exception &e) {
        compleks::logger::error("Exception occured: "s + e.what());
    }

    compleks::logger::info("Exiting...");

    return EXIT_SUCCESS;
}
