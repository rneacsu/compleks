#pragma once

#include <memory>

#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include "../world/object.hxx"
#include "shape.hxx"

namespace compleks {

class body : public object {
public:
    float mass = 0;
    std::unique_ptr<btRigidBody> rigid_body;

    body(std::string mesh_id);
    ~body();

    void create_body(
        std::shared_ptr<shape> collision_shape, bool inertia = true);
    void remove_body(void);
    void update_all(double delta) override;

    void set_position(glm::vec3 position);
    void set_velocity(glm::vec3 velocity);
    glm::vec3 get_velocity(void);

protected:
    std::unique_ptr<btDefaultMotionState> motion_state;
    std::shared_ptr<shape> collision_shape;
};

} // namespace compleks
