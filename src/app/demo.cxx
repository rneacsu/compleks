#include "demo.hxx"
#include <iostream>

#include "scenes/scenes.hxx"
#include "shapes/shapes.hxx"

demo::demo()
{
    get_meshes().add("cube", "res/meshes/box.obj");
    get_meshes().add("gate", "res/meshes/gate.obj");
    get_meshes().add("goal", "res/meshes/hyperisocahedron.obj");

    get_shapes().add("plane", []() { return std::make_unique<plane_shape>(); });
    get_shapes().add("cube", []() { return std::make_unique<cube_shape>(); });
    get_shapes().add("gate", []() { return std::make_unique<gate_shape>(); });

    load_scene(1);
    screen_overlay.animate(overlay::FADE_OUT);
}

void demo::load_scene(int scene_num)
{
    current_scene_num = scene_num;
    std::unique_ptr<compleks::scene> s;

    compleks::logger::info("Loading scene " + std::to_string(scene_num));

    get_world().current_scene = nullptr;
    switch (scene_num) {
    case 1:
        s = std::make_unique<scene1>();
        break;
    case 2:
        s = std::make_unique<scene2>();
        break;
    case 3:
        s = std::make_unique<scene3>();
        break;
    case 4:
        s = std::make_unique<scene4>();
        break;
    case 5:
        s = std::make_unique<scene5>();
        break;
    }

    if (!s) {
        return;
    }

    auto p = s->bodies.find("goal");
    goal = p != s->bodies.end() ? p->second : nullptr;

    get_world().current_scene = std::move(s);
}

void demo::key_down(int key, int mods)
{
    compleks::engine::key_down(key, mods);
    int scene_num = key - GLFW_KEY_0;

    if (screen_overlay.is_running()) {
        return;
    }

    if (scene_num > 0 && scene_num <= NUM_SCENES
        && scene_num != current_scene_num) {
        screen_overlay.set_callback(
            [this, scene_num]() { load_scene(scene_num); });
        screen_overlay.animate(overlay::FADE_IN_OUT);
    }

    if (key == GLFW_KEY_R) {
        screen_overlay.set_callback(
            [this]() { load_scene(current_scene_num); });
        screen_overlay.animate(overlay::FADE_IN_OUT);
    }
}

void demo::render(double delta)
{
    screen_overlay.update(delta);
    engine::render(delta);

    // Check goal
    if (goal) {
        if (!screen_overlay.is_running()) {
            glm::vec3 pos = goal->pos;
            float r = 3;

            if (glm::length(get_world().camera.get_position() - pos) < r) {
                screen_overlay.set_callback([this]() {
                    if (++current_scene_num > NUM_SCENES) {
                        current_scene_num = 1;
                    }
                    load_scene(current_scene_num);
                });
                screen_overlay.animate(overlay::FADE_IN_OUT);
            }
        }

        static float t = 0;
        t += (float)delta / 2;
        if (t > glm::two_pi<float>()) {
            t -= glm::two_pi<float>();
        }

        goal->quat = glm::quat({ 2 * t, 3 * t, t });
    }
}
