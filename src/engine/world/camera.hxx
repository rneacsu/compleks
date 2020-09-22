#ifndef __COMPLEKS_WORLD_CAMERA_HXX
#define __COMPLEKS_WORLD_CAMERA_HXX

#include <glm/glm.hpp>

#include "../input/input_listener.hxx"

namespace compleks {

class camera : public input_listener {
public:
    camera();

    void update(double delta);

    glm::mat4 get_view_matrix();
    glm::mat4 get_projection_matrix(int width, int height);
    glm::vec3 get_position();
    void set_position(glm::vec3 pos);
    void set_orientation(float yaw, float pitch, float roll);
    void set_enabled(bool);
    void reset();

    void key_down(int key, int mods) override;
    void key_up(int key, int mods) override;
    void mouse_move(float dx, float dy) override;
    void mouse_scroll(float delta) override;

private:
    float yaw, pitch, roll, fov;
    glm::vec3 pos, front, up, right;
    char walk_front, walk_right, elevate, tilt;
    bool sprint = false;
    bool enabled = false;
};

} // namespace compleks

#endif // __COMPLEKS_WORLD_CAMERA_HXX
