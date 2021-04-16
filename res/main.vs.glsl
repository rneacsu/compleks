#version 400 core

layout(location = 0) in vec3 v_vertex;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_tex_coord;

uniform mat4 model_matrix;
uniform mat4 projection_view_matrix;

out vec3 f_vertex;
out vec3 f_normal;
out vec2 f_tex_coord;

void main()
{
    mat4 mvp = projection_view_matrix * model_matrix;

    gl_Position = mvp * vec4(v_vertex, 1.0);

    f_vertex = (model_matrix * vec4(v_vertex, 1)).xyz;
    f_normal = normalize(mat3(transpose(inverse(model_matrix))) * v_normal);
    f_tex_coord = v_tex_coord;
}
