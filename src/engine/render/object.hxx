#ifndef __COMPLEKS_WORLD_OBJECT_HXX
#define __COMPLEKS_WORLD_OBJECT_HXX

#include <memory>
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
    glm::vec4 color;

    object(std::string id);

    virtual void update(double delta);
    void render(glm::mat4 transform = glm::mat4(1));
    glm::mat4 get_model_matrix(void);
    glm::mat4 get_view_matrix(void);

protected:
    std::string id;

private:
};

} // namespace compleks

#endif // __COMPLEKS_WORLD_OBJECT_HXX
