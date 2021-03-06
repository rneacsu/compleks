#ifndef __COMPLEKS_INPUT_INPUT_WINDOW_HXX
#define __COMPLEKS_INPUT_INPUT_WINDOW_HXX

#include <list>
#include <map>
#include <utility>

#include "input_listener.hxx"
#include "window.hxx"

namespace compleks {

class input_window : public window, public input_listener {
public:
    input_window();

    void add_listener(input_listener *);
    void remove_listener(input_listener *);
    void set_cursor(bool enabled);

    bool cursor_enabled = true;

private:
    static std::map<GLFWwindow *,
        std::pair<input_window *, std::list<input_listener *>>>
        listeners;
    static void key_callback(GLFWwindow *, int, int, int, int);
    static void mouse_button_callback(GLFWwindow *, int, int, int);
    static void mouse_move_callback(GLFWwindow *, double, double);
    static void mouse_enter_callback(GLFWwindow *, int);
    static void mouse_scroll_callback(GLFWwindow *, double, double);
    static void window_resize_callback(GLFWwindow *, int, int);

    double old_x, old_y;
};

}

#endif // __COMPLEKS_INPUT_INPUT_WINDOW_HXX
