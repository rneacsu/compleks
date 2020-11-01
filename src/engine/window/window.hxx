#ifndef COMPLEKS_CORE_WINDOW_HXX
#define COMPLEKS_CORE_WINDOW_HXX

#include <mutex>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../image/image.hxx"

namespace compleks {

class window {
public:
    char const *const DEFAULT_TITLE = "Compleks";
    int const DEFAULT_WIDTH = 640;
    int const DEFAULT_HEIGHT = 480;

    window();
    ~window();

    void config(char const *title, int w, int h);
    void run(void);
    void set_icon(const image &img);
    void set_icon(int res_id);
    void toggle_fullscreen(void);

    virtual void render(double delta);

protected:
    GLFWwindow *ctx;
    bool fullscreen = false;
    int width, height, x, y;

private:
    static std::mutex lock;
    static int instances;
    double old_time = 0;

    static void glfw_error_func(int, const char *);
};

}

#endif
