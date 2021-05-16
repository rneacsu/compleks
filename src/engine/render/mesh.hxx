#ifndef __COMPLEKS_RENDER_MESH_HXX
#define __COMPLEKS_RENDER_MESH_HXX

#include <memory>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.hxx"

namespace compleks {

class mesh {

    struct material {
        std::shared_ptr<texture> diffuse_tex, specular_tex, ambient_tex,
            emissive_tex;
        glm::vec3 diffuse_color, specular_color, ambient_color, emissive_color;
        float shininess;
    };

    struct shape {
        int material_idx;
        unsigned int num_indices;
        unsigned int start_index;
    };

public:
    mesh(std::string path);
    mesh(
        std::vector<glm::vec3> v,
        std::vector<glm::vec3> n,
        std::vector<glm::vec2> t,
        std::vector<unsigned int> i);
    ~mesh();

    void render(glm::vec4 color = glm::vec4(1));
    void render_simple(void);

private:
    GLuint vao, vertices_buf, normals_buf, tex_coords_buf, indices_buf;
    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> tex_coords;
    std::vector<unsigned int> indices;
    std::vector<material> materials;
    std::vector<shape> shapes;

    material default_material;

    void create_buffers(void);
    void create_default_material(void);
    void update_material(material &m);
    void draw_shape(shape &s);
};

} // namespace compleks

#endif // __COMPLEKS_RENDER_MESH_HXX
