#pragma once

#include <memory>

#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include "../render/object.hxx"
#include "shape.hxx"

namespace compleks {

class portal;

class body : public object {
public:
    glm::vec3 old_pos;
    std::unique_ptr<btRigidBody> rigid_body;
    std::shared_ptr<body> clone_body;
    std::shared_ptr<portal> clone_portal;

    body(std::string id, float mass = 0, bool inertia = true);
    body(const body &b);
    ~body();

    void create_body(std::unique_ptr<shape> s);
    void remove_body(void);
    void update(double delta) override;

    void set_position(glm::vec3 pos, glm::vec3 old);
    void set_quat(glm::quat q);
    void set_scale(glm::vec3 s);
    void set_velocity(glm::vec3 velocity);
    glm::vec3 get_velocity(void);
    void set_gravity(bool enabled);
    void set_collisions(bool enabled);
    float get_mass(void);

    void create_clone(std::shared_ptr<portal> p);
    void remove_clone(void);

protected:
    float mass;
    bool inertia;

    std::unique_ptr<btDefaultMotionState> motion_state;
    std::shared_ptr<shape> collision_shape;
};

} // namespace compleks
