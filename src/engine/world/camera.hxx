#ifndef __COMPLEKS_WORLD_CAMERA_HXX
#define __COMPLEKS_WORLD_CAMERA_HXX

#include <glm/glm.hpp>

#include "../physics/body.hxx"
#include "../window/input_listener.hxx"

namespace compleks {

class camera : public input_listener {
public:
    enum view_type { CENTER, LEFT, RIGHT };
    enum camera_type { NORMAL, FREEFORM };

    const float EYE_HEIGHT = 1.7f;
    const float RADIUS = 0.3f;
    const float HEIGHT = 1.8f;
    const float NEAR_CLIP = 0.001f;
    const float FAR_CLIP = 100.0f;
    const float MASS = 80.0f;
    const float JUMP_FORCE = 5.0f;

    camera();

    void update(double delta);

    glm::mat4 get_view_matrix(view_type t = CENTER);
    glm::mat4 get_projection_matrix(int width, int height);

    glm::vec3 get_old_position();
    glm::vec3 get_position();
    void set_position(glm::vec3 pos, glm::vec3 old);

    glm::vec3 get_orientation();
    void set_orientation(glm::vec3 euler);

    glm::vec3 get_velocity();
    void set_velocity(glm::vec3 v);

    glm::vec3 get_front();

    void reset();

    camera_type get_camera_type(void);
    void set_camera_type(camera_type type);

    void jump(void);

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
    float sensitivity = 0.005f;
    float max_speed = 3.0f;
    float sprint_multiplier = 2;
    float eye_distance = 0.30f;
    enum camera_type cam_type;
};

} // namespace compleks

#endif // __COMPLEKS_WORLD_CAMERA_HXX
