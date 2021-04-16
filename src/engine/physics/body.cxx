#include "body.hxx"

#include "../core/engine.hxx"
#include "../utils/logger.hxx"
#include "../world/portal.hxx"

namespace compleks {

body::body(std::string id, float mass, bool inertia)
    : object(id)
    , mass(mass)
    , inertia(inertia)
{
    old_pos = pos;
}

body::~body()
{
    remove_body();
}

body::body(const body &b)
    : object(b)
{
    mass = b.mass;
    old_pos = b.old_pos;
    inertia = b.inertia;
    if (b.collision_shape) {
        collision_shape = b.collision_shape;
    }
    if (b.rigid_body) {
        create_body(nullptr);
    }
}

void body::create_body(std::unique_ptr<shape> s)
{
    if (rigid_body) {
        remove_body();
    }

    if (s) {
        collision_shape = std::move(s);
    }

    collision_shape->get_shape().setLocalScaling(physics::to_bt(scale));

    motion_state = std::make_unique<btDefaultMotionState>(
        physics::to_bt(pos, quat));

    btVector3 local_inertia = btVector3(0, 0, 0);
    if (inertia) {
        collision_shape->get_shape().calculateLocalInertia(mass, local_inertia);
    }

    btRigidBody::btRigidBodyConstructionInfo ci(
        mass,
        motion_state.get(),
        &collision_shape->get_shape(),
        local_inertia);

    rigid_body = std::make_unique<btRigidBody>(ci);
    rigid_body->setUserPointer(this);

    engine::get_physics().get_world().addRigidBody(rigid_body.get());
}

void body::remove_body(void)
{
    if (!rigid_body) {
        return;
    }
    engine::get_physics().get_world().removeRigidBody(rigid_body.get());

    motion_state = nullptr;
    rigid_body = nullptr;
}

void body::update(double)
{
    if (!rigid_body) {
        return;
    }

    btTransform transform;

    transform = rigid_body->getWorldTransform();
    glm::vec3 new_pos = physics::to_glm(transform.getOrigin());
    glm::quat new_quat = physics::to_glm(transform.getRotation());

    if (clone_body) {
        transform = clone_body->rigid_body->getWorldTransform();
        glm::vec3 clone_new_pos = physics::to_glm(transform.getOrigin());
        glm::quat clone_new_quat = physics::to_glm(transform.getRotation());

        float dist = glm::length(new_pos - pos);
        float clone_dist = glm::length(clone_new_pos - clone_body->pos);

        if (clone_dist && clone_dist > dist) {
            clone_body->old_pos = clone_body->pos;
            clone_body->pos = clone_new_pos;
            clone_body->quat = clone_new_quat;
            set_position(
                clone_portal->get_target()->pass(clone_new_pos),
                clone_portal->get_target()->pass(clone_body->old_pos));
            set_quat(clone_portal->get_target()->pass(clone_new_quat));

            return;
        } else {
            clone_body->set_position(
                clone_portal->pass(new_pos),
                clone_portal->pass(pos));
            clone_body->set_quat(clone_portal->pass(new_quat));
        }
    }

    old_pos = pos;
    pos = new_pos;
    quat = new_quat;
}

void body::set_position(glm::vec3 p, glm::vec3 old)
{
    old_pos = old;
    pos = p;
    if (rigid_body) {
        auto &t = rigid_body->getWorldTransform();
        t.setOrigin(physics::to_bt(p));
        rigid_body->setWorldTransform(t);
    }
}

void body::set_quat(glm::quat q)
{
    quat = q;
    if (rigid_body) {
        auto &t = rigid_body->getWorldTransform();
        t.setRotation(physics::to_bt(q));
        rigid_body->setWorldTransform(t);
    }
}

void body::set_scale(glm::vec3 s)
{
    scale = s;
    if (collision_shape) {
        collision_shape->get_shape().setLocalScaling(physics::to_bt(s));
    }
}

void body::set_velocity(glm::vec3 v)
{
    rigid_body->setLinearVelocity(physics::to_bt(v));
}

glm::vec3 body::get_velocity(void)
{
    return physics::to_glm(rigid_body->getLinearVelocity());
}

void body::set_gravity(bool enabled)
{
    if (enabled) {
        rigid_body->setGravity(engine::get_physics().get_world().getGravity());
    } else {
        rigid_body->setGravity(btVector3(0, 0, 0));
    }
}

float body::get_mass(void)
{
    return mass;
}

void body::set_collisions(bool enabled)
{
    if (enabled) {
        rigid_body->setCollisionFlags(
            rigid_body->getCollisionFlags()
            & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
    } else {
        rigid_body->setCollisionFlags(
            rigid_body->getCollisionFlags()
            | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    }
}

void body::create_clone(std::shared_ptr<portal> p)
{
    if (clone_portal == p) {
        return;
    }

    clone_portal = p;
    clone_body = std::make_shared<body>(*this);
    clone_body->set_position(p->pass(pos), p->pass(old_pos));
    clone_body->set_quat(p->pass(quat));
}

void body::remove_clone(void)
{
    clone_body = nullptr;
    clone_portal = nullptr;
}

} // namespace compleks
