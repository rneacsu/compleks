#ifndef __COMPLEKS_WORLD_OBJECT_HXX
#define __COMPLEKS_WORLD_OBJECT_HXX

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../core/engine.hxx"
#include "../render/mesh.hxx"
#include "../render/program.hxx"

namespace compleks {

class object {
public:
    glm::vec3 pos;
    glm::vec3 scale;
    glm::quat quat;

    object(std::string mesh_id);

    virtual void update(double delta);
    void update_all(double delta);

    void render(engine &en, program &prog, glm::mat4 transform = glm::mat4(1));

    glm::mat4 get_model_matrix(void);
    glm::mat4 get_view_matrix(void);

protected:
    std::vector<object> children;

private:
    std::string mesh_id;
};

} // namespace compleks

#endif // __COMPLEKS_WORLD_OBJECT_HXX
