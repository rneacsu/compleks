#pragma once

#include <map>
#include <memory>
#include <string>

#include <bullet/btBulletDynamicsCommon.h>

#include "../utils/library.hxx"

namespace compleks {

class physics {
public:
    physics();
    ~physics();

    library<std::unique_ptr<btCollisionShape>> &get_shapes();
    btDiscreteDynamicsWorld &get_dynamics_world();
    void update(double delta);

private:
    std::unique_ptr<btDefaultCollisionConfiguration> collision_configuration;
    std::unique_ptr<btCollisionDispatcher> dispatcher;
    std::unique_ptr<btBroadphaseInterface> broadphase;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
    std::unique_ptr<btDiscreteDynamicsWorld> dynamics_world;
};

} // namespace compleks
