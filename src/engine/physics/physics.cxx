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

    dynamics_world = std::make_unique<btDiscreteDynamicsWorld>(dispatcher.get(),
        broadphase.get(), solver.get(), collision_configuration.get());

    dynamics_world->setGravity(btVector3(0, -10, 0));
}

physics::~physics()
{
    logger::info("Destroying physics engine");
}

btDiscreteDynamicsWorld &physics::get_dynamics_world()
{
    return *dynamics_world;
}

void physics::update(double delta)
{
    dynamics_world->stepSimulation(btScalar(delta), 0);
}

} // namespace compleks
