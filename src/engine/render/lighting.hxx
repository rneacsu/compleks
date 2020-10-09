#ifndef __COMPLEKS_RENDER_LIGHTING_HXX
#define __COMPLEKS_RENDER_LIGHTING_HXX

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "program.hxx"
#include "texture.hxx"

namespace compleks {

class lighting {
public:
    struct light {
        glm::vec3 pos;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 attenuation;
        glm::vec3 direction;
        float cut_off;
    };

    lighting();
    ~lighting();

    void add_light(light *light);
    void remove_light(light *light);
    void update(program &p, glm::vec3 eye);

private:
    const unsigned int MAX_LIGHTS = 5;

    std::vector<light *> lights;
    texture noise_texture;
};

} // namespace compleks

#endif // __COMPLEKS_RENDER_LIGHTING_HXX
