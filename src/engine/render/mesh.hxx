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
        std::unique_ptr<texture> diffuse, specular, ambient, emissive;
        float shininess;
    };

    struct shape {
        int material_idx;
        unsigned int num_indices;
        unsigned int start_index;
    };

public:
    mesh(std::string path);
    ~mesh();

    void render();

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
};

} // namespace compleks

#endif // __COMPLEKS_RENDER_MESH_HXX
