#include "demo.hxx"
#include <iostream>

#include "shapes/shapes.hxx"

demo::demo()
{
    get_meshes().add("ground", "res/meshes/ground.obj");
    get_meshes().add("box", "res/meshes/box.obj");
    get_meshes().add("gate", "res/meshes/gate.obj");
    get_meshes().add("goal", "res/meshes/hyperisocahedron.obj");
    get_meshes().add("ball", "res/meshes/ball.obj");
    get_meshes().add("tree", "res/meshes/tree.obj");

    get_shapes().add("gate", []() { return std::make_unique<gate_shape>(); });
    get_shapes().add("tree", []() {
        return std::make_unique<compleks::shape>(
            std::make_unique<btCylinderShape>(btVector3(2.0, 10.0, 2.0)));
    });

    get_skybox().load("res/skybox", "png");

    load_scene(1);
    screen_overlay.animate(overlay::FADE_OUT);
}

void demo::load_scene(int scene_num)
{
    current_scene_num = scene_num;
    std::unique_ptr<compleks::scene> s;
    std::string path = "res/scenes/scene" + std::to_string(scene_num) + ".json";

    compleks::logger::info("Loading scene " + path);

    get_world().current_scene = nullptr;

    s = std::make_unique<compleks::simple_scene>(path);
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
        restart_scene();
    }
}

void demo::restart_scene(void)
{
    screen_overlay.set_callback([this]() { load_scene(current_scene_num); });
    screen_overlay.animate(overlay::FADE_IN_OUT);
}

void demo::render(double delta)
{
    screen_overlay.update(delta);
    engine::render(delta);

    // Check goal
    if (goal) {
        if (!screen_overlay.is_running()) {
            glm::vec3 pos = goal->pos;
            float r = 2;

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

    // Check if too far from level area
    if (glm::length(get_world().camera.get_position()) > 25) {
        restart_scene();
    }
}

void demo::mouse_down(int button)
{
    bool f = focused;
    engine::mouse_down(button);

    if (!f) {
        return;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        auto &c = get_world().camera;
        auto b = std::make_shared<compleks::body>("ball", 5.0f);

        b->scale *= 0.45f;
        b->pos = c.get_position() + c.get_front() * 1.5f;
        b->color = { 252 / 255.0f, 194 / 255.0f, 3 / 255.0f, 1 };
        b->create_body(get_shapes().get("sphere")());

        glm::vec3 force;
        force = c.get_front() * (10.0f * b->get_mass());
        b->rigid_body->applyCentralImpulse(compleks::physics::to_bt(force));

        get_world().dynamic_objects.push_back(b);
    }
}
