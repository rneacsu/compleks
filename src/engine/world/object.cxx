#include "object.hxx"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "../render/mesh_library.hxx"

namespace compleks
{
    
object::object(std::string mesh_id) : mesh_id(mesh_id)
{
    pos = {0, 0, 0};
    scale = {1, 1, 1};
    yaw = pitch = roll = 0;
}

void object::render(program &prog, double delta, glm::mat4 transform)
{
    update(delta);

    transform = glm::translate(transform, pos);
    transform *= glm::yawPitchRoll(yaw, pitch, roll);
    transform = glm::scale(transform, scale);

    prog.set("model_matrix", transform);

    if (!mesh_id.empty()) {
        mesh_library::get(mesh_id).render(prog);
    }

    for (auto &child : children) {
        child.render(prog, delta, transform);
    }
}

void object::update(double)
{

}

} // namespace compleks
