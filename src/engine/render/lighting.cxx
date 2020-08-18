#include "lighting.hxx"

#include <string>

#include <glm/gtc/type_ptr.hpp>

#include "../image/image.hxx"
#include "../utils/logger.hxx"
#include "../utils/resource.hxx"

namespace compleks {

lighting::lighting()
    : noise_texture(resource("res/noise.png"))
{
}

lighting::~lighting()
{
}

void lighting::add_light(light* l)
{
    if (lights.size() == MAX_LIGHTS) {
        logger::warn(
            "Cannot add more than " + std::to_string(MAX_LIGHTS) + " ligths");

        return;
    }
    lights.push_back(l);
}

void lighting::remove_light(light* l)
{
    lights.erase(std::remove(lights.begin(), lights.end(), l), lights.end());
}

void lighting::update(program& p, glm::vec3 eye)
{
    p.set("eye", eye);

    p.set("light_noise", noise_texture);

    for (unsigned int i = 0; i < lights.size(); i++) {
        std::string prefix = "lights[" + std::to_string(i) + "].";

        p.set(prefix + "on", 1);
        p.set(prefix + "pos", lights[i]->pos);
        p.set(prefix + "ambient", lights[i]->ambient);
        p.set(prefix + "diffuse", lights[i]->diffuse);
        p.set(prefix + "specular", lights[i]->specular);
        p.set(prefix + "attenuation", lights[i]->attenuation);
        p.set(prefix + "direction", lights[i]->direction);
        p.set(prefix + "cut_off", lights[i]->cut_off);
    }

    for (size_t i = lights.size(); i < MAX_LIGHTS; i++) {
        std::string prefix = "lights[" + std::to_string(i) + "].";

        p.set(prefix + "on", 0);
    }
}

} // namespace compleks
