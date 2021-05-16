#version 400 core

layout(location = 0) in vec3 v_vertex;

uniform mat4 projection_view_matrix;

out vec3 f_tex_coord;

void main()
{
    f_tex_coord = v_vertex;
    vec4 pos = projection_view_matrix * vec4(v_vertex, 1.0);
    gl_Position = pos.xyww;
}
