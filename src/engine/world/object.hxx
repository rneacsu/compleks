#ifndef __COMPLEKS_WORLD_OBJECT_HXX
#define __COMPLEKS_WORLD_OBJECT_HXX

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "../render/program.hxx"

namespace compleks
{
    
class object {
public:
    glm::vec3 pos;
    glm::vec3 scale;
    float yaw, pitch, roll;

    object(std::string mesh_id = "");
    
    virtual void update(double delta);

    void render(program &prog, double delta, glm::mat4 transform = glm::mat4(1));
protected:
    std::vector<object> children;
private:
    std::string mesh_id;
};

} // namespace compleks


#endif // __COMPLEKS_WORLD_OBJECT_HXX