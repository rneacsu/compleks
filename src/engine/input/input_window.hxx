#ifndef __COMPLEKS_INPUT_INPUT_WINDOW_HXX
#define __COMPLEKS_INPUT_INPUT_WINDOW_HXX

#include <map>
#include <list>
#include <utility>
#include <tuple>

#include "../core/window.hxx"
#include "input_listener.hxx"

namespace compleks
{

class input_window : public window, public input_listener {
public:
    input_window();

    void add_listener(input_listener *);
    void remove_listener(input_listener *);
    void set_cursor(bool enabled);

    void key_down(int key, int mods) override;
    void key_up(int key, int mods) override;
    void mouse_down(int button) override;
    void mouse_up(int button) override;
    void mouse_move(float, float) override;
    void mouse_enter() override;
    void mouse_leave() override;
    void mouse_scroll(float) override;
    
protected:
    bool cursor_enabled = true;
private:
    static std::map<
        GLFWwindow *,
        std::pair<input_window *, std::list<input_listener *>>
    > listeners;
    static void key_callback(GLFWwindow *, int, int, int, int);
    static void mouse_button_callback(GLFWwindow *, int, int, int);
    static void mouse_move_callback(GLFWwindow *, double, double);
    static void mouse_enter_callback(GLFWwindow *, int);
    static void mouse_scroll_callback(GLFWwindow *, double, double);

    double old_x, old_y;

    void log_key(bool down, int key, int mods);
};
    
}

#endif // __COMPLEKS_INPUT_INPUT_WINDOW_HXX