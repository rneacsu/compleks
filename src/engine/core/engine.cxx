#include "engine.hxx"

#include <stdexcept>
#include <string>

#include "../render/lighting.hxx"
#include "../render/mesh.hxx"
#include "../render/program.hxx"
#include "../utils/logger.hxx"

namespace compleks {

engine thread_local *engine::instance;

engine::engine()
{
    if (instance) {
        throw std::runtime_error("Only one window can be created per thread");
    }
    logger::info("Creating engine");
    instance = this;
}

engine::~engine()
{
    logger::info("Shutting down engine");
    instance = nullptr;
}

program &engine::get_program()
{
    return instance->prog;
}

library<mesh> &engine::get_meshes()
{
    return instance->meshes;
}

lighting &engine::get_lighting()
{
    return instance->light;
}

}
