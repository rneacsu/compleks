#include "body.hxx"

#include "../core/engine.hxx"

namespace compleks {

body::body(std::string mesh_id)
    : object(mesh_id)
{
}

body::~body()
{
    remove_body();
}

void body::create_body(std::shared_ptr<shape> s, bool inertia)
{
    if (rigid_body) {
        return;
    }

    collision_shape = s;

    motion_state = std::make_unique<btDefaultMotionState>(
        btTransform(btQuaternion(quat.x, quat.y, quat.z, quat.w),
            btVector3(pos.x, pos.y, pos.z)));

    btVector3 local_inertia = btVector3(0, 0, 0);
    if (inertia) {
        collision_shape->get_shape()->calculateLocalInertia(
            mass, local_inertia);
    }

    btRigidBody::btRigidBodyConstructionInfo ci(
        mass, motion_state.get(), collision_shape->get_shape(), local_inertia);

    rigid_body = std::make_unique<btRigidBody>(ci);
    rigid_body->setUserPointer(this);

    engine::get_physics().get_dynamics_world().addRigidBody(rigid_body.get());
}

void body::remove_body(void)
{
    if (!rigid_body) {
        return;
    }
    engine::get_physics().get_dynamics_world().removeRigidBody(
        rigid_body.get());

    motion_state = nullptr;
    rigid_body = nullptr;
}

void body::update_all(double)
{
    if (!rigid_body) {
        return;
    }

    btTransform transform;
    transform = rigid_body->getWorldTransform();
    btVector3 new_pos = transform.getOrigin();
    btQuaternion new_quat = transform.getRotation();

    pos.x = new_pos.x();
    pos.y = new_pos.y();
    pos.z = new_pos.z();

    quat.x = new_quat.x();
    quat.y = new_quat.y();
    quat.z = new_quat.z();
    quat.w = new_quat.w();
}

void body::set_position(glm::vec3 p)
{
    auto &t = rigid_body->getWorldTransform();
    t.setOrigin(btVector3(p.x, p.y, p.z));
    rigid_body->setWorldTransform(t);
}

void body::set_velocity(glm::vec3 v)
{
    rigid_body->setLinearVelocity(btVector3(v.x, v.y, v.z));
}

glm::vec3 body::get_velocity(void)
{
    btVector3 v = rigid_body->getLinearVelocity();

    return glm::vec3(v.x(), v.y(), v.z());
}

} // namespace compleks
