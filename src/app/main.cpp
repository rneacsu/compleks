#include <exception>
#include <string>

#include <compleks.h>

#include "worlds/demo_world.hxx"

using namespace std::literals;

int main(int, char *[])
{
    compleks::logger::info("Starting...");

    try {
        demo_world win;

        win.set_icon(
            compleks::image(compleks::resource("APP_WINDOW_ICON", "IMAGE")));
        win.run();

    } catch (std::exception &e) {
        compleks::logger::error("Exception occured: "s + e.what());
    }

    compleks::logger::info("Exiting...");

    return EXIT_SUCCESS;
}
