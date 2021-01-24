#include "demo_world.hxx"
#include <iostream>

#include "scenes/scene1.hxx"
#include "scenes/scene2.hxx"
#include "scenes/scene3.hxx"
#include "scenes/scene4.hxx"
#include "scenes/scene5.hxx"
#include "scenes/scene6.hxx"

demo_world::demo_world()
{
    load_mesh("cube", "res/cube.obj");
    load_mesh("gate", "res/gate.obj");

    load_scene<scene1>();
    current_scene_num = 1;
}

void demo_world::key_down(int key, int mods)
{
    compleks::world::key_down(key, mods);
    int scene_num = key - GLFW_KEY_0;
    if (scene_num > 0 && scene_num < 7 && scene_num != current_scene_num) {
        current_scene_num = scene_num;

        compleks::logger::info(
            "Loading scene " + std::to_string(current_scene_num));

        switch (scene_num) {
        case 1:
            load_scene<scene1>();
            break;
        case 2:
            load_scene<scene2>();
            break;
        case 3:
            load_scene<scene3>();
            break;
        case 4:
            load_scene<scene4>();
            break;
        case 5:
            load_scene<scene5>();
            break;
        case 6:
            load_scene<scene6>();
            break;
        }
    }
}
