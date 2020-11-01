#ifndef __COMPLEKS_RENDER_LIGHTING_HXX
#define __COMPLEKS_RENDER_LIGHTING_HXX

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "texture.hxx"

namespace compleks {

class lighting {
public:
    struct light {
        bool off = false;
        glm::vec3 pos = glm::vec3();
        glm::vec3 ambient = glm::vec3(0.25f);
        glm::vec3 diffuse = glm::vec3(1);
        glm::vec3 specular = glm::vec3(1);
        glm::vec3 attenuation = glm::vec3(1, 0, 0);
        glm::vec3 direction;
        float cut_off = 0;
    };

    lighting();
    ~lighting();

    void add_light(std::shared_ptr<light> l);
    void remove_light(std::shared_ptr<light> l);
    void update(glm::vec3 eye);

private:
    const unsigned int MAX_LIGHTS = 5;

    std::vector<std::shared_ptr<light>> lights;
    texture noise_texture;
};

} // namespace compleks

#endif // __COMPLEKS_RENDER_LIGHTING_HXX
