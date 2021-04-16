#include "physics.hxx"

#include "../utils/logger.hxx"

namespace compleks {

physics::physics()
{
    logger::info("Creating physics engine");

    collision_configuration
        = std::make_unique<btDefaultCollisionConfiguration>();
    dispatcher = std::make_unique<btCollisionDispatcher>(
        collision_configuration.get());
    broadphase = std::make_unique<btDbvtBroadphase>();
    solver = std::make_unique<btSequentialImpulseConstraintSolver>();

    dynamics_world = std::make_unique<btDiscreteDynamicsWorld>(
        dispatcher.get(),
        broadphase.get(),
        solver.get(),
        collision_configuration.get());

    dynamics_world->setGravity(btVector3(0, -10, 0));
}

physics::~physics()
{
    logger::info("Destroying physics engine");
}

btDiscreteDynamicsWorld &physics::get_world()
{
    return *dynamics_world;
}

void physics::update(double delta)
{
    dynamics_world->stepSimulation(btScalar(delta), 0);
}

body *physics::ray_test(glm::vec3 start, glm::vec3 end)
{
    btVector3 ray_start = to_bt(start);
    btVector3 ray_end = to_bt(end);
    btCollisionWorld::ClosestRayResultCallback ray_callback(ray_start, ray_end);

    dynamics_world->rayTest(ray_start, ray_end, ray_callback);

    if (ray_callback.hasHit()) {
        return (body *)ray_callback.m_collisionObject->getUserPointer();
    }

    return nullptr;
}

std::shared_ptr<body> physics::ray_test(
    glm::vec3 start,
    glm::vec3 end,
    std::vector<std::shared_ptr<body>> objects)
{
    body *p = ray_test(start, end);

    if (p) {
        for (auto &obj : objects) {
            if (obj.get() == p) {
                return obj;
            }
        }
    }

    return nullptr;
}

bool physics::contact(btCollisionObject &o1, btCollisionObject &o2)
{
    contact_callback callback;

    dynamics_world->contactPairTest(&o1, &o2, callback);

    return callback.hit;
}

glm::vec3 physics::to_glm(btVector3 v)
{
    return glm::vec3(v.x(), v.y(), v.z());
}

glm::quat physics::to_glm(btQuaternion q)
{
    return glm::quat(q.w(), q.x(), q.y(), q.z());
}

btVector3 physics::to_bt(glm::vec3 v)
{
    return btVector3(v.x, v.y, v.z);
}

btQuaternion physics::to_bt(glm::quat q)
{
    return btQuaternion(q.x, q.y, q.z, q.w);
}

btTransform physics::to_bt(glm::vec3 p, glm::quat q)
{
    return btTransform(to_bt(q), to_bt(p));
}

} // namespace compleks
