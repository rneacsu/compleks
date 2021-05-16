#include "overlay.hxx"

#include <algorithm>

overlay::overlay()
{
    overlay_obj = std::make_unique<compleks::object>("quad");
    overlay_obj->color = { 0.90f, 0.90f, 0.90f, 0 };
    overlay_obj->scale = { 10000, 10000, 1 };
    compleks::engine::get().get_world().hud.push_back(overlay_obj);
    running = false;
    duration = 1.0f;
}

overlay::~overlay()
{
    auto &hud = compleks::engine::get().get_world().hud;
    hud.erase(std::remove(hud.begin(), hud.end(), overlay_obj), hud.end());
}

bool overlay::is_running()
{
    return running;
}

void overlay::set_duration(float d)
{
    duration = d;
}

void overlay::set_callback(std::function<void()> c)
{
    callback = c;
}

void overlay::animate(animation_type t)
{
    if (is_running()) {
        return;
    }

    type = t;
    elapsed_time = 0;
    running = true;
}

void overlay::update(double delta)
{
    if (!is_running()) {
        return;
    }

    float alpha = 0;

    if (type == FADE_IN_OUT && callback) {
        if (elapsed_time < duration / 2
            && elapsed_time + (float)delta >= duration / 2) {
            callback();
        }
    }

    elapsed_time += (float)delta;

    if (elapsed_time > duration) {
        elapsed_time = duration;
        running = false;
    }

    switch (type) {
    case FADE_IN:
        alpha = elapsed_time / duration;
        break;
    case FADE_OUT:
        alpha = (duration - elapsed_time) / duration;
        break;
    case FADE_IN_OUT:
        if (elapsed_time < duration / 2) {
            alpha = elapsed_time / duration * 2;
        } else {
            alpha = (duration - elapsed_time) / duration * 2;
        }
        break;
    }

    overlay_obj->color.a = alpha;
}
