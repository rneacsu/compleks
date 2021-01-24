#include "scene.hxx"

#include "../utils/logger.hxx"
#include "world.hxx"

namespace compleks {

scene::scene(compleks::world &world)
    : world(world)
{
}

scene::~scene()
{
    world.objects.clear();
    world.portals.clear();
    world.camera.reset();
    engine::get_lighting().clear();
}

} // namespace compleks
