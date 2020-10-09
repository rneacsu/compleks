#include "mesh.hxx"

#include <filesystem>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <glm/gtc/type_ptr.hpp>
#include <tiny_obj_loader.h>

#include "../utils/logger.hxx"

namespace compleks {

mesh::mesh(std::string path)
{
    logger::info("Importing mesh " + path);

    tinyobj::ObjReader reader;
    bool ret = reader.ParseFromFile(path);

    std::string warn = reader.Warning();
    std::string err = reader.Error();

    if (!warn.empty()) {
        logger::warn(warn);
    }
    if (!err.empty()) {
        logger::error(err);
    }
    if (!ret) {
        throw std::runtime_error("Could not import mesh " + path);
    }

    tinyobj::attrib_t attr = reader.GetAttrib();
    unsigned int num_vertices = 0;
    bool create_def_mat = false;
    for (auto &shape : reader.GetShapes()) {
        struct shape s;

        s.material_idx = shape.mesh.material_ids[0];
        if (s.material_idx < 0) {
            create_def_mat = true;
        }
        s.start_index = num_vertices;

        for (auto idx : shape.mesh.indices) {
            vertices.push_back({ attr.vertices[3 * idx.vertex_index + 0],
                attr.vertices[3 * idx.vertex_index + 1],
                attr.vertices[3 * idx.vertex_index + 2] });
            normals.push_back({ attr.normals[3 * idx.normal_index + 0],
                attr.normals[3 * idx.normal_index + 1],
                attr.normals[3 * idx.normal_index + 2] });
            tex_coords.push_back({ attr.texcoords[2 * idx.texcoord_index + 0],
                attr.texcoords[2 * idx.texcoord_index + 1] });
            indices.push_back(num_vertices++);
        }
        s.num_indices = num_vertices - s.start_index;
        shapes.push_back(s);
    }

    std::string dir = std::filesystem::path(path).parent_path().string() + "/";

    for (auto &m : reader.GetMaterials()) {
        material mat;

        if (!m.diffuse_texname.empty()) {
            mat.diffuse = std::make_unique<texture>(
                image(resource(dir + m.diffuse_texname)));
        } else {
            mat.diffuse = std::make_unique<texture>(glm::make_vec3(m.diffuse));
        }

        if (!m.specular_texname.empty()) {
            mat.specular = std::make_unique<texture>(
                image(resource(dir + m.specular_texname)));
        } else {
            mat.specular
                = std::make_unique<texture>(glm::make_vec3(m.specular));
        }

        if (!m.ambient_texname.empty()) {
            mat.ambient = std::make_unique<texture>(
                image(resource(dir + m.ambient_texname)));
        } else if (!m.diffuse_texname.empty()) {
            mat.ambient = std::make_unique<texture>(
                image(resource(dir + m.diffuse_texname)));
        } else {
            mat.ambient = std::make_unique<texture>(glm::make_vec3(m.ambient));
        }

        if (!m.emissive_texname.empty()) {
            mat.emissive = std::make_unique<texture>(
                image(resource(dir + m.emissive_texname)));
        } else {
            mat.emissive
                = std::make_unique<texture>(glm::make_vec3(m.emission));
        }

        mat.shininess = m.shininess;

        materials.push_back(std::move(mat));
    }

    create_buffers();

    if (create_def_mat) {
        create_default_material();
    }
}

void mesh::create_buffers()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertices_buf);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(),
        vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &normals_buf);
    glBindBuffer(GL_ARRAY_BUFFER, normals_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(),
        normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &tex_coords_buf);
    glBindBuffer(GL_ARRAY_BUFFER, tex_coords_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords[0]) * tex_coords.size(),
        tex_coords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &indices_buf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(),
        indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void mesh::create_default_material(void)
{
    default_material.diffuse
        = std::make_unique<texture>(glm::vec3(1.0f, 1.0f, 1.0f));
    default_material.specular = std::make_unique<texture>();
    default_material.ambient = std::make_unique<texture>();
    default_material.emissive = std::make_unique<texture>();
    default_material.shininess = 30;
}

mesh::~mesh()
{
    logger::info("Releasing mesh");
    glDeleteBuffers(1, &vertices_buf);
    glDeleteBuffers(1, &normals_buf);
    glDeleteBuffers(1, &tex_coords_buf);
    glDeleteBuffers(1, &indices_buf);
    glDeleteVertexArrays(1, &vao);
}

void mesh::render(program &prog)
{
    glBindVertexArray(vao);
    for (auto &s : shapes) {
        material *m;
        if (s.material_idx < 0) {
            m = &default_material;
        } else {
            m = &materials[s.material_idx];
        }
        prog.set("material.diffuse", *m->diffuse);
        prog.set("material.specular", *m->specular);
        prog.set("material.ambient", *m->ambient);
        prog.set("material.emissive", *m->emissive);
        prog.set("material.shininess", m->shininess);

        glDrawElements(GL_TRIANGLES, s.num_indices, GL_UNSIGNED_INT,
            (void *)((size_t)s.start_index * sizeof(indices[0])));
    }
    glBindVertexArray(0);
}
}
