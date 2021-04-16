#ifndef __COMPLEKS_WORLD_PORTAL_HXX
#define __COMPLEKS_WORLD_PORTAL_HXX

#include <memory>

#include "../render/object.hxx"
#include "camera.hxx"

namespace compleks {

class portal : public object {
public:
    std::vector<std::weak_ptr<portal>> sub_portals;

    portal();
    ~portal();

    glm::mat4 modify_view_matrix(glm::mat4 view);
    glm::mat4 modify_proj_matrix(glm::mat4 view, glm::mat4 proj);

    void set_target(std::shared_ptr<portal> p);
    std::shared_ptr<portal> get_target(void);

    bool pass(camera &c);
    bool pass(body &c);
    glm::vec3 pass(glm::vec3 pos);
    bool pass(glm::vec3 start, glm::vec3 end);
    glm::quat pass(glm::quat quat);
    glm::vec3 pass_rel(glm::vec3 v);

    bool contact(body &c);

    glm::vec3 get_normal(bool inverse = false);
    glm::vec4 get_clip_plane(bool inverse = false);

private:
    std::weak_ptr<portal> target;
};

} // namespace compleks

#endif // __COMPLEKS_WORLD_PORTAL_HXX
