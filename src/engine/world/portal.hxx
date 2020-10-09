#ifndef __COMPLEKS_WORLD_PORTAL_HXX
#define __COMPLEKS_WORLD_PORTAL_HXX

#include "object.hxx"

namespace compleks {

class camera;
class world;

class portal : public object {
public:
    portal();
    ~portal();

    glm::mat4 modify_view_matrix(glm::mat4 view);
    glm::mat4 modify_proj_matrix(glm::mat4 view, glm::mat4 proj);
    void set_target(std::shared_ptr<portal> p);
    std::shared_ptr<portal> get_target(void);
    bool pass(camera &c);

private:
    std::weak_ptr<portal> target;
};

} // namespace compleks

#endif // __COMPLEKS_WORLD_PORTAL_HXX
