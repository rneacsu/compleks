#include "mesh.hxx"

#include <filesystem>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <glm/gtc/type_ptr.hpp>
#include <tiny_obj_loader.h>

#include "../core/engine.hxx"

namespace compleks {

mesh::mesh(std::string path)
{
    logger::info("Loading mesh " + path);

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
    unsigned int num_indices = 0;

    for (auto &shape : reader.GetShapes()) {
        struct shape s;

        s.material_idx = shape.mesh.material_ids[0];
        s.start_index = num_indices;

        for (auto idx : shape.mesh.indices) {
            vertices.push_back(
                glm::make_vec3(attr.vertices.data() + 3 * idx.vertex_index));
            normals.push_back(
                glm::make_vec3(attr.normals.data() + 3 * idx.normal_index));
            if (idx.texcoord_index >= 0) {
                tex_coords.push_back(glm::make_vec2(
                    attr.texcoords.data() + 2 * idx.texcoord_index));
            } else {
                tex_coords.push_back(glm::vec2(0));
            }
            indices.push_back(num_indices++);
        }
        s.num_indices = num_indices - s.start_index;
        shapes.push_back(s);
    }

    std::string dir = std::filesystem::path(path).parent_path().string() + "/";

    for (auto &m : reader.GetMaterials()) {
        material mat;

        if (!m.diffuse_texname.empty()) {
            mat.diffuse_tex = std::make_shared<texture>(
                image(resource(dir + m.diffuse_texname)));
        }
        mat.diffuse_color = glm::make_vec3(m.diffuse);

        if (!m.specular_texname.empty()) {
            mat.specular_tex = std::make_shared<texture>(
                image(resource(dir + m.specular_texname)));
        }
        mat.specular_color = glm::make_vec3(m.specular);

        if (!m.ambient_texname.empty()) {
            mat.ambient_tex = std::make_shared<texture>(
                image(resource(dir + m.ambient_texname)));
        } else if (mat.diffuse_tex) {
            mat.ambient_tex = mat.diffuse_tex;
        }
        mat.ambient_color = glm::make_vec3(m.ambient);

        if (!m.emissive_texname.empty()) {
            mat.emissive_tex = std::make_shared<texture>(
                image(resource(dir + m.emissive_texname)));
        }
        mat.emissive_color = glm::make_vec3(m.emission);

        mat.shininess = m.shininess;

        materials.push_back(std::move(mat));
    }

    create_buffers();
    create_default_material();
}

mesh::mesh(
    std::vector<glm::vec3> v,
    std::vector<glm::vec3> n,
    std::vector<glm::vec2> t,
    std::vector<unsigned int> i)
    : vertices(v)
    , normals(n)
    , tex_coords(t)
    , indices(i)
{
    logger::info("Creating primitive mesh");

    struct shape s;

    s.material_idx = -1;
    s.start_index = 0;
    s.num_indices = (unsigned int)indices.size();
    shapes.push_back(s);

    create_buffers();
    create_default_material();
}

void mesh::create_buffers()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertices_buf);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_buf);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices[0]) * vertices.size(),
        vertices.data(),
        GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &normals_buf);
    glBindBuffer(GL_ARRAY_BUFFER, normals_buf);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(normals[0]) * normals.size(),
        normals.data(),
        GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &tex_coords_buf);
    glBindBuffer(GL_ARRAY_BUFFER, tex_coords_buf);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(tex_coords[0]) * tex_coords.size(),
        tex_coords.data(),
        GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &indices_buf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices[0]) * indices.size(),
        indices.data(),
        GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void mesh::create_default_material(void)
{
    default_material.diffuse_color = glm::vec3(0.8f, 0.8f, 0.8f);
    default_material.specular_color = glm::vec3(0.05f, 0.05f, 0.05f);
    default_material.ambient_color = glm::vec3(1.0f, 1.0f, 1.0f);
    default_material.emissive_color = glm::vec3(0.0f, 0.0f, 0.0f);
    default_material.shininess = 324;
}

mesh::~mesh()
{
    logger::info("Destroying mesh");
    glDeleteBuffers(1, &vertices_buf);
    glDeleteBuffers(1, &normals_buf);
    glDeleteBuffers(1, &tex_coords_buf);
    glDeleteBuffers(1, &indices_buf);
    glDeleteVertexArrays(1, &vao);
}

void mesh::render(glm::vec4 color)
{
    program &p = engine::get_program();

    glBindVertexArray(vao);
    for (auto &s : shapes) {
        material *m;
        if (s.material_idx < 0) {
            m = &default_material;
        } else {
            m = &materials[s.material_idx];
        }

        if (m->diffuse_tex) {
            p.set("material.diffuse.tex_on", true);
            p.set("material.diffuse.tex", *m->diffuse_tex);
        } else {
            p.set("material.diffuse.tex_on", false);
            p.set("material.diffuse.color", m->diffuse_color);
        }

        if (m->specular_tex) {
            p.set("material.specular.tex_on", true);
            p.set("material.specular.tex", *m->specular_tex);
        } else {
            p.set("material.specular.tex_on", false);
            p.set("material.specular.color", m->specular_color);
        }

        if (m->ambient_tex) {
            p.set("material.ambient.tex_on", true);
            p.set("material.ambient.tex", *m->ambient_tex);
        } else {
            p.set("material.ambient.tex_on", false);
            p.set("material.ambient.color", m->ambient_color);
        }

        if (m->emissive_tex) {
            p.set("material.emissive.tex_on", true);
            p.set("material.emissive.tex", *m->emissive_tex);
        } else {
            p.set("material.emissive.tex_on", false);
            p.set("material.emissive.color", m->emissive_color);
        }

        p.set("material.shininess", m->shininess);
        p.set("material.tint", glm::vec3(color));
        p.set("material.alpha", color.a);

        glDrawElements(
            GL_TRIANGLES,
            s.num_indices,
            GL_UNSIGNED_INT,
            (void *)((size_t)s.start_index * sizeof(indices[0])));
    }
    glBindVertexArray(0);
}

}
