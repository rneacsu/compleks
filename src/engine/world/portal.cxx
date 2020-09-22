#include "portal.hxx"

#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "world.hxx"

namespace compleks {

portal::portal()
    : object("quad")
{
}

portal::~portal()
{
}

glm::mat4 portal::modify_view_matrix(glm::mat4 view)
{
    auto t = target.lock();
    if (!t) {
        return view;
    }

    return view * get_view_matrix()
        * glm::rotate(
            glm::mat4(1), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::inverse(t->get_view_matrix());
}

glm::mat4 portal::modify_proj_matrix(glm::mat4 view, glm::mat4 proj)
{
    glm::vec3 normal = quat * glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec4 clipPlane(normal, -glm::dot(normal, pos));

    clipPlane = glm::transpose(glm::inverse(view)) * clipPlane;

    if (clipPlane.w > 0.0f)
        return proj;

    glm::vec4 q = glm::inverse(proj)
        * glm::vec4(glm::sign(clipPlane.x), glm::sign(clipPlane.y), 1.0f, 1.0f);
    glm::vec4 c = clipPlane * (2.0F / glm::dot(clipPlane, q));
    proj = glm::row(proj, 2, c - glm::row(proj, 3));

    return proj;
}

void portal::set_target(std::shared_ptr<portal> p)
{
    target = p;
}

std::shared_ptr<portal> portal::get_target(void)
{
    return target.lock();
}

} // namespace compleks
