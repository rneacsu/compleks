#include "portal.hxx"

#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "../core/engine.hxx"
#include "../utils/logger.hxx"

namespace compleks {

portal::portal()
    : object("quad")
{
}

portal::~portal()
{
}

glm::vec3 portal::get_normal(bool inverse)
{
    return quat * glm::vec3(0, 0, inverse ? -1 : 1);
}

glm::vec4 portal::get_clip_plane(bool inverse)
{
    glm::vec3 n = get_normal(inverse);

    return glm::vec4(n, -glm::dot(n, pos));
}

glm::mat4 portal::modify_view_matrix(glm::mat4 view)
{
    auto t = get_target();
    if (!t) {
        return view;
    }

    return view * get_view_matrix()
        * glm::mat4_cast(glm::quat({ 0.0f, glm::pi<float>(), 0.0f }))
        * glm::inverse(t->get_view_matrix());
}

glm::mat4 portal::modify_proj_matrix(glm::mat4 view, glm::mat4 proj)
{
    glm::vec4 clip_plane
        = glm::transpose(glm::inverse(view)) * get_clip_plane(true);

    if (clip_plane.w > 0.0f)
        return proj;

    glm::vec4 q = glm::inverse(proj)
        * glm::vec4(glm::sign(clip_plane.x),
                    glm::sign(clip_plane.y),
                    1.0f,
                    1.0f);
    glm::vec4 c = clip_plane * (2.0F / glm::dot(clip_plane, q));
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

glm::vec3 portal::pass(glm::vec3 p)
{
    auto t = get_target();
    return t->pos
        + t->quat * glm::quat({ 0.0f, glm::pi<float>(), 0.0f })
        * glm::inverse(quat) * (p - pos);
}

glm::vec3 portal::pass_rel(glm::vec3 v)
{
    auto t = get_target();
    return t->quat * glm::quat({ 0.0f, glm::pi<float>(), 0.0f })
        * glm::inverse(quat) * v;
}

glm::quat portal::pass(glm::quat q)
{
    auto t = get_target();
    return t->quat * glm::quat({ 0.0f, glm::pi<float>(), 0.0f })
        * glm::inverse(quat) * q;
}

bool portal::pass(glm::vec3 start, glm::vec3 end)
{
    glm::vec3 n = get_normal();

    if (start == end) {
        return false;
    }

    if (glm::dot(n, start - pos) < 0 || glm::dot(n, end - pos) > 0) {
        return false;
    }

    glm::vec3 x, y, v;
    x = quat * glm::vec3(1, 0, 0);
    y = quat * glm::vec3(0, 1, 0);

    v = end - pos;
    if (glm::abs(glm::dot(x, v)) <= scale.x / 2
        && glm::abs(glm::dot(y, v)) <= scale.y / 2) {
        return true;
    }

    v = start - pos;
    if (glm::abs(glm::dot(x, v)) <= scale.x / 2
        && glm::abs(glm::dot(y, v)) <= scale.y / 2) {
        return true;
    }

    return false;
}

bool portal::pass(camera &c)
{
    auto t = get_target();
    if (!t) {
        return false;
    }

    glm::vec3 n = get_normal();
    glm::vec3 old = c.get_old_position();
    glm::vec3 cur = c.get_position();

    if (old == cur) {
        return false;
    }

    float err = c.NEAR_CLIP * 2;
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

    glm::vec3 new_pos = pass(cur);

    float delta = glm::dot(n, cur - exit);
    if (delta > 0) {
        new_pos -= n * delta;
    }
    c.set_position(new_pos, pass(old));

    glm::vec3 tn = t->get_normal();
    glm::vec3 cur_o, target_o, cam_o;
    cur_o = { glm::atan(n.y, glm::sqrt(n.x * n.x + n.z * n.z)),
              glm::atan(n.z, n.x),
              0.0f };
    target_o = { glm::atan(tn.y, glm::sqrt(tn.x * tn.x + tn.z * tn.z)),
                 glm::atan(tn.z, tn.x),
                 0.0f };
    cam_o = c.get_orientation();

    cam_o.x += target_o.x + cur_o.x;
    cam_o.y += target_o.y - cur_o.y + glm::pi<float>();

    c.set_orientation(cam_o);
    c.set_velocity(pass_rel(c.get_velocity()));

    return true;
}

bool portal::pass(body &obj)
{
    auto t = get_target();
    if (!t) {
        return false;
    }

    glm::vec3 old = obj.old_pos;
    glm::vec3 cur = obj.pos;

    if (!pass(old, cur)) {
        return false;
    }

    obj.set_position(pass(cur), pass(old));
    obj.set_quat(pass(obj.quat));
    obj.set_velocity(pass_rel(obj.get_velocity()));

    return true;
}

bool portal::contact(body &b)
{
    btBoxShape portal_shape(btVector3(scale.x / 2, scale.y / 2, 0.01f));
    btCollisionObject co;

    co.setCollisionShape(&portal_shape);
    co.setWorldTransform(physics::to_bt(pos, quat));

    return engine::get_physics().contact(co, *b.rigid_body);
}

} // namespace compleks
