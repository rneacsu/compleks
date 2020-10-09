#include "portal.hxx"

#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "camera.hxx"
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

bool portal::pass(camera &c)
{
    auto t = target.lock();
    if (!t) {
        return false;
    }

    glm::vec3 n = quat * glm::vec3(0, 0, 1);
    glm::vec3 old = c.get_old_position();
    glm::vec3 cur = c.get_position();

    if (old == cur) {
        return false;
    }

    float err = 0.002f; // 2 * camera near
    glm::vec3 entrance = pos + n * err;
    glm::vec3 exit = pos - n * err;

    if (glm::dot(n, old - entrance) < 0 || glm::dot(n, cur - entrance) > 0) {
        return false;
    }

    glm::vec3 v = cur - pos;

    glm::vec3 x, y;
    x = quat * glm::vec3(1, 0, 0);
    y = quat * glm::vec3(0, 1, 0);

    if (glm::abs(glm::dot(x, v)) > scale.x / 2
        || glm::abs(glm::dot(y, v)) > scale.y / 2) {
        return false;
    }

    float delta = glm::dot(n, cur - exit);
    if (delta < 0) {
        c.set_position(cur + t->pos - pos);
    } else {
        c.set_position(cur + t->pos - pos - n * delta);
    }

    return true;
}

} // namespace compleks
