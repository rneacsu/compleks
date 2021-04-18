#pragma once

#include <functional>

#include <compleks.h>

class overlay {
public:
    overlay();
    ~overlay();

    typedef enum { FADE_IN, FADE_OUT, FADE_IN_OUT } animation_type;

    void update(double delta);
    void set_duration(float duration);
    void set_callback(std::function<void()> callback);

    void animate(animation_type t);

    bool is_running();

private:
    animation_type type;
    float elapsed_time;
    float duration;
    bool running;
    std::function<void()> callback;

    std::shared_ptr<compleks::object> overlay_obj;
};
