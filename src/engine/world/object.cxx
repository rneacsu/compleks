#include "object.hxx"

#include <glm/gtc/matrix_transform.hpp>

#include "../core/engine.hxx"

namespace compleks {

object::object(std::string mesh_id)
    : mesh_id(mesh_id)
{
    pos = { 0, 0, 0 };
    scale = { 1, 1, 1 };
    quat = glm::quat(glm::vec3(0, 0, 0));
}

glm::mat4 object::get_model_matrix(void)
{
    return glm::scale(get_view_matrix(), scale);
}

glm::mat4 object::get_view_matrix(void)
{
    return glm::translate(glm::mat4(1), pos) * glm::mat4_cast(quat);
}

void object::render(glm::mat4 transform)
{
    transform *= get_model_matrix();

    engine::get_program().set("model_matrix", transform);

    if (!mesh_id.empty()) {
        engine::get_meshes().get(mesh_id).render();
    }

    for (auto &child : children) {
        child.render(transform);
    }
}

void object::update(double)
{
}

void object::update_all(double delta)
{
    // Update self
    update(delta);
    // Update children
    for (auto &child : children) {
        child.update_all(delta);
    }
}

} // namespace compleks
