#include "mesh.hxx"

#include <filesystem>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "../utils/logger.hxx"

namespace compleks {

mesh::mesh(char const *path)
{
    logger::info("Importing mesh " + std::string(path));

    Assimp::Importer importer;
    aiScene const *scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_JoinIdenticalVertices
            | aiProcess_GenSmoothNormals);

    if (!scene) {
        throw std::runtime_error(importer.GetErrorString());
    }

    unsigned int num_indices = 0, num_vertices = 0;
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh *m = scene->mMeshes[i];
        object o;
        aiVector3D zero_vect(0, 0, 0);

        o.material_idx = m->mMaterialIndex;
        o.num_indices = m->mNumFaces * 3;
        o.start_index = num_indices;
        objects.push_back(o);

        for (unsigned int j = 0; j < m->mNumVertices; j++) {
            aiVector3D &v = m->mVertices[j];
            aiVector3D &n = m->mNormals[j];
            aiVector3D &t
                = m->HasTextureCoords(0) ? m->mTextureCoords[0][j] : zero_vect;

            vertices.push_back({ v.x, v.y, v.z });
            normals.push_back({ n.x, n.y, n.z });
            tex_coords.push_back({ t.x, t.y });
        }

        for (unsigned int j = 0; j < m->mNumFaces; j++) {
            aiFace &f = m->mFaces[j];

            indices.push_back(num_vertices + f.mIndices[0]);
            indices.push_back(num_vertices + f.mIndices[1]);
            indices.push_back(num_vertices + f.mIndices[2]);
        }

        num_vertices += m->mNumVertices;
        num_indices += o.num_indices;
    }

    std::string dir = std::filesystem::path(path).parent_path().string() + "/";

    for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial *m = scene->mMaterials[i];
        material mat;
        aiColor3D color;
        aiString str;

        if (m->GetTextureCount(aiTextureType_DIFFUSE) > 0
            && m->GetTexture(aiTextureType_DIFFUSE, 0, &str) == AI_SUCCESS) {
            mat.diffuse = std::make_unique<texture>(
                image(resource((dir + str.C_Str()).c_str())));
        } else if (m->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
            mat.diffuse = std::make_unique<texture>(
                glm::vec4(color.r, color.g, color.b, 1.0));
        }

        if (m->GetTextureCount(aiTextureType_SPECULAR) > 0
            && m->GetTexture(aiTextureType_SPECULAR, 0, &str) == AI_SUCCESS) {
            mat.specular = std::make_unique<texture>(
                image(resource((dir + str.C_Str()).c_str())));
        } else if (m->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS) {
            mat.specular = std::make_unique<texture>(
                glm::vec4(color.r, color.g, color.b, 1.0));
        }

        if (m->GetTextureCount(aiTextureType_AMBIENT) > 0
            && m->GetTexture(aiTextureType_AMBIENT, 0, &str) == AI_SUCCESS) {
            mat.ambient = std::make_unique<texture>(
                image(resource((dir + str.C_Str()).c_str())));
        } else if (m->GetTextureCount(aiTextureType_DIFFUSE) > 0
            && m->GetTexture(aiTextureType_DIFFUSE, 0, &str) == AI_SUCCESS) {
            mat.ambient = std::make_unique<texture>(
                image(resource((dir + str.C_Str()).c_str())));
        } else if (m->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS) {
            mat.ambient = std::make_unique<texture>(
                glm::vec4(color.r, color.g, color.b, 1.0));
        }

        if (m->GetTextureCount(aiTextureType_EMISSIVE) > 0
            && m->GetTexture(aiTextureType_EMISSIVE, 0, &str) == AI_SUCCESS) {
            mat.emissive = std::make_unique<texture>(
                image(resource((dir + str.C_Str()).c_str())));
        } else if (m->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS) {
            mat.emissive = std::make_unique<texture>(
                glm::vec4(color.r, color.g, color.b, 1.0));
        }

        m->Get(AI_MATKEY_SHININESS, mat.shininess);

        materials.push_back(std::move(mat));
    }

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
    for (auto &o : objects) {
        material &m = materials[o.material_idx];
        prog.set("material.diffuse", *m.diffuse);
        prog.set("material.specular", *m.specular);
        prog.set("material.ambient", *m.ambient);
        prog.set("material.emissive", *m.emissive);
        prog.set("material.shininess", m.shininess);

        glDrawElements(GL_TRIANGLES, o.num_indices, GL_UNSIGNED_INT,
            (void *)((size_t)o.start_index * sizeof(indices[0])));
    }
    glBindVertexArray(0);
}

}