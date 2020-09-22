#ifndef __COMPLEKS_RENDER_MESH_HXX
#define __COMPLEKS_RENDER_MESH_HXX

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "program.hxx"

namespace compleks {

class mesh {

    struct material {
        std::unique_ptr<texture> diffuse, specular, ambient, emissive;
        float shininess;
    };

    struct object {
        unsigned int material_idx;
        unsigned int num_indices;
        unsigned int start_index;
    };

public:
    mesh(std::string path);
    ~mesh();

    void render(program &);

private:
    GLuint vao, vertices_buf, normals_buf, tex_coords_buf, indices_buf;
    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> tex_coords;
    std::vector<unsigned int> indices;
    std::vector<material> materials;
    std::vector<object> objects;

    void create_buffers();
};

} // namespace compleks

#endif // __COMPLEKS_RENDER_MESH_HXX
