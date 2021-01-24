#include "lighting.hxx"

#include <queue>
#include <string>

#include <glm/gtc/type_ptr.hpp>

#include "../core/engine.hxx"
#include "../image/bayer.hxx"
#include "../utils/resource.hxx"

namespace compleks {

lighting::lighting()
    : noise_texture(bayer(), texture::mode::NEAREST)
{
}

lighting::~lighting()
{
}

void lighting::add_light(std::shared_ptr<light> l)
{
    lights.push_back(l);
}

void lighting::remove_light(std::shared_ptr<light> l)
{
    lights.erase(std::remove(lights.begin(), lights.end(), l), lights.end());
}

void lighting::clear()
{
    lights.clear();
}

void lighting::update(glm::vec3 eye)
{
    program &p = engine::get_program();

    p.set("eye", eye);
    p.set("light_noise", noise_texture);

    std::priority_queue<std::pair<float, light *>> pq;

    for (auto &l : lights) {
        pq.emplace(-glm::distance(eye, l->pos), l.get());
    }

    unsigned int i = 0;
    while (!pq.empty() && i < MAX_LIGHTS) {
        light *l = pq.top().second;
        pq.pop();

        if (l->off) {
            continue;
        }

        std::string prefix = "lights[" + std::to_string(i++) + "].";

        p.set(prefix + "off", 0);
        p.set(prefix + "pos", l->pos);
        p.set(prefix + "ambient", l->ambient);
        p.set(prefix + "diffuse", l->diffuse);
        p.set(prefix + "specular", l->specular);
        p.set(prefix + "attenuation", l->attenuation);
        p.set(prefix + "direction", l->direction);
        p.set(prefix + "cut_off", l->cut_off);
    }

    for (; i < MAX_LIGHTS; i++) {
        std::string prefix = "lights[" + std::to_string(i) + "].";

        p.set(prefix + "off", 1);
    }
}

} // namespace compleks
