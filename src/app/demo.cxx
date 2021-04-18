#include "demo.hxx"
#include <iostream>

#include "scenes/scenes.hxx"
#include "shapes/shapes.hxx"

demo::demo()
{
    get_meshes().add("cube", "res/meshes/box.obj");
    get_meshes().add("gate", "res/meshes/gate.obj");

    get_shapes().add("plane", []() { return std::make_unique<plane_shape>(); });
    get_shapes().add("cube", []() { return std::make_unique<cube_shape>(); });
    get_shapes().add("gate", []() { return std::make_unique<gate_shape>(); });

    get_world().current_scene = std::make_unique<scene1>();
    current_scene_num = 1;
    screen_overlay.animate(overlay::FADE_OUT);
}

std::unique_ptr<compleks::scene> demo::create_scene(int scene_num)
{
    switch (scene_num) {
    case 1:
        return std::make_unique<scene1>();
    case 2:
        return std::make_unique<scene2>();
    case 3:
        return std::make_unique<scene3>();
    case 4:
        return std::make_unique<scene4>();
    case 5:
        return std::make_unique<scene5>();
    }

    return nullptr;
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
        screen_overlay.set_callback([this, scene_num]() {
            current_scene_num = scene_num;

            compleks::logger::info(
                "Loading scene " + std::to_string(current_scene_num));

            get_world().current_scene = nullptr;
            get_world().current_scene = create_scene(current_scene_num);
        });
        screen_overlay.animate(overlay::FADE_IN_OUT);
    }

    if (key == GLFW_KEY_R) {
        screen_overlay.set_callback([this]() {
            compleks::logger::info(
                "Reloading scene " + std::to_string(current_scene_num));

            get_world().current_scene = nullptr;
            get_world().current_scene = create_scene(current_scene_num);
        });
        screen_overlay.animate(overlay::FADE_IN_OUT);
    }
}

void demo::render(double delta)
{
    screen_overlay.update(delta);
    engine::render(delta);

    // Check goal
    if (!screen_overlay.is_running()) {
        auto &j = get_world().current_scene->config;
        if (j.contains("goal")) {
            glm::vec3 pos = compleks::scene_loader::get_pos(j["goal"]["pos"]);
            float r = j["goal"]["radius"];

            if (glm::length(get_world().camera.get_position() - pos) < r) {
                screen_overlay.set_callback([this]() {
                    if (++current_scene_num > NUM_SCENES) {
                        current_scene_num = 1;
                    }

                    compleks::logger::info(
                        "Loading scene " + std::to_string(current_scene_num));

                    get_world().current_scene = nullptr;
                    get_world().current_scene = create_scene(current_scene_num);
                });
                screen_overlay.animate(overlay::FADE_IN_OUT);
            }
        }
    }
}
