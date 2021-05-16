#version 400 core

in vec3 f_tex_coord;

uniform samplerCube skybox;

out vec4 output_color;

void main()
{
    output_color = texture(skybox, f_tex_coord);
}
