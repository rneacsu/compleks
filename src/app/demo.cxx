#include "demo.hxx"
#include <iostream>

#include "scenes/scene1.hxx"
#include "scenes/scene2.hxx"
#include "scenes/scene3.hxx"
#include "scenes/scene4.hxx"
#include "scenes/scene5.hxx"
#include "scenes/scene6.hxx"

demo::demo()
{
    get_meshes().add("cube", "res/cube.obj");
    get_meshes().add("gate", "res/gate.obj");

    get_world().current_scene = std::make_unique<scene1>();
    current_scene_num = 1;

    overlay = std::make_shared<compleks::object>("quad");
    overlay->pos = { 25, 25, 0 };
    // overlay->pos = { 0, 0, 0 };
    overlay->scale = { 50, 50, 1 };
    overlay->color = { 0.85f, 0.85f, 0.85f, overlay_alpha };

    get_world().hud.push_back(overlay);
}

void demo::key_down(int key, int mods)
{
    compleks::engine::key_down(key, mods);
    int scene_num = key - GLFW_KEY_0;
    if (scene_num > 0 && scene_num < 7 && scene_num != current_scene_num) {
        current_scene_num = scene_num;

        compleks::logger::info(
            "Loading scene " + std::to_string(current_scene_num));

        get_world().current_scene = nullptr;

        std::unique_ptr<compleks::scene> new_scene;
        switch (scene_num) {
        case 1:
            new_scene = std::make_unique<scene1>();
            break;
        case 2:
            new_scene = std::make_unique<scene2>();
            break;
        case 3:
            new_scene = std::make_unique<scene3>();
            break;
        case 4:
            new_scene = std::make_unique<scene4>();
            break;
        case 5:
            new_scene = std::make_unique<scene5>();
            break;
        case 6:
            new_scene = std::make_unique<scene6>();
            break;
        }

        get_world().current_scene = std::move(new_scene);
    }
}


void demo::render(double delta)
{
    engine::render(delta);

    overlay_alpha += (float)(2 * delta);
    if (overlay_alpha > 2) {
        overlay_alpha -= 2;
    }

    if (overlay_alpha > 1) {
        overlay->color.a = 2 - overlay_alpha;
    } else {
        overlay->color.a = overlay_alpha;
    }
}
