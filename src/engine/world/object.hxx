#ifndef __COMPLEKS_WORLD_OBJECT_HXX
#define __COMPLEKS_WORLD_OBJECT_HXX

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace compleks {

class object {
public:
    glm::vec3 pos;
    glm::vec3 scale;
    glm::quat quat;
    glm::vec3 color;

    object(std::string mesh_id);

    virtual void update(double delta);
    virtual void update_all(double delta);

    void render(glm::mat4 transform = glm::mat4(1));

    glm::mat4 get_model_matrix(void);
    glm::mat4 get_view_matrix(void);

protected:
    std::string mesh_id;
    std::vector<object> children;

private:
};

} // namespace compleks

#endif // __COMPLEKS_WORLD_OBJECT_HXX
