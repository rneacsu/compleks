#pragma once

#include <map>
#include <memory>
#include <string>

#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../utils/library.hxx"
#include "body.hxx"

namespace compleks {

class physics {
public:
    physics();
    ~physics();

    btDiscreteDynamicsWorld &get_world();
    void update(double delta);
    bool contact(btCollisionObject &s1, btCollisionObject &s2);
    std::shared_ptr<body> ray_test(
        glm::vec3 start,
        glm::vec3 end,
        std::vector<std::shared_ptr<body>> objects);

    body *ray_test(glm::vec3 start, glm::vec3 end);

    static glm::vec3 to_glm(btVector3 v);
    static glm::quat to_glm(btQuaternion q);
    static btVector3 to_bt(glm::vec3 v);
    static btQuaternion to_bt(glm::quat q);
    static btTransform to_bt(glm::vec3 p, glm::quat q);

private:
    class contact_callback : public btCollisionWorld::ContactResultCallback {
    public:
        bool hit = false;

        btScalar addSingleResult(
            btManifoldPoint &,
            const btCollisionObjectWrapper *,
            int,
            int,
            const btCollisionObjectWrapper *,
            int,
            int)
        {
            hit = true;

            return 0;
        }
    };

    std::unique_ptr<btDefaultCollisionConfiguration> collision_configuration;
    std::unique_ptr<btCollisionDispatcher> dispatcher;
    std::unique_ptr<btBroadphaseInterface> broadphase;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
    std::unique_ptr<btDiscreteDynamicsWorld> dynamics_world;
};

} // namespace compleks
