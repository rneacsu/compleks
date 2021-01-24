#ifndef __COMPLEKS_WORLD_CAMERA_HXX
#define __COMPLEKS_WORLD_CAMERA_HXX

#include <glm/glm.hpp>

#include "../physics/body.hxx"
#include "../window/input_listener.hxx"

namespace compleks {

class camera : public input_listener {
public:
    enum view_type { CENTER, LEFT, RIGHT };

    camera();

    void update(double delta);

    glm::mat4 get_view_matrix(view_type t = CENTER);
    glm::mat4 get_projection_matrix(int width, int height);

    glm::vec3 get_old_position();
    glm::vec3 get_position();
    void set_position(glm::vec3 pos);

    glm::vec3 get_orientation();
    void set_orientation(glm::vec3 euler);

    glm::vec3 get_velocity();
    void set_velocity(glm::vec3 v);

    void set_enabled(bool);
    void reset();

    void key_down(int key, int mods) override;
    void key_up(int key, int mods) override;
    void mouse_move(float dx, float dy) override;
    void mouse_scroll(float delta) override;

private:
    float yaw, pitch, roll, fov;
    glm::vec3 pos, old_pos, front, up, right;
    char walk_front, walk_right, elevate, tilt;
    body body;
    bool sprint = false;
    bool enabled = false;
    float sensitivity = 0.005f;
    float max_speed = 3.0f;
    float sprint_multiplier = 2;
    float eye_distance = 0.30f;
    const float eye_height = 1.7f;
};

} // namespace compleks

#endif // __COMPLEKS_WORLD_CAMERA_HXX
