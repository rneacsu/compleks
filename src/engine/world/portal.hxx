#ifndef __COMPLEKS_WORLD_PORTAL_HXX
#define __COMPLEKS_WORLD_PORTAL_HXX

#include <memory>

#include "camera.hxx"
#include "object.hxx"

namespace compleks {

class portal : public object {
public:
    portal();
    ~portal();

    glm::mat4 modify_view_matrix(glm::mat4 view);
    glm::mat4 modify_proj_matrix(glm::mat4 view, glm::mat4 proj);
    void set_target(std::shared_ptr<portal> p);
    std::shared_ptr<portal> get_target(void);
    std::vector<std::weak_ptr<portal>> &get_sub_portals(void);
    bool pass(camera &c);

private:
    std::weak_ptr<portal> target;
    std::vector<std::weak_ptr<portal>> sub_portals;
};

} // namespace compleks

#endif // __COMPLEKS_WORLD_PORTAL_HXX
