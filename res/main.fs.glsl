#version 400 core

#define MAX_LIGHTS 5

struct light_t {
    bool on;
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
    vec3 direction;
    float cut_off;
};

struct texture_t {
    bool tex_on;
    sampler2D tex;
    vec3 color;
};

struct material_t {
    texture_t diffuse;
    texture_t specular;
    texture_t ambient;
    texture_t emissive;
    float shininess;
    vec3 tint;
    float alpha;
};

uniform bool light_on;
uniform light_t lights[MAX_LIGHTS];
uniform sampler2D light_noise;

uniform material_t material;

uniform vec3 eye;

in vec3 f_vertex;
in vec3 f_normal;
in vec2 f_tex_coord;

uniform bool clip;
uniform vec4 clip_plane;

uniform vec2 fog;

out vec4 output_color;

vec3 add_lights(vec3 color)
{
    vec3 V = normalize(eye - f_vertex);
    bool dither = false;

    if (!light_on) {
        vec3 ambient;
        if (material.ambient.tex_on) {
            ambient = vec3(texture(material.ambient.tex, f_tex_coord));
        } else {
            ambient = material.ambient.color;
        }

        color += ambient * material.tint;

        return color;
    }

    for (int i = 0; i < MAX_LIGHTS; i++) {
        if (!lights[i].on) {
            continue;
        }

        vec3 L = normalize(lights[i].pos - f_vertex);

        bool spot_enable = lights[i].cut_off != 0;
        float spot_angle = dot(-L, lights[i].direction);
        float spot_limit = cos(lights[i].cut_off);

        if (!spot_enable || spot_angle > spot_limit) {
            vec3 H = normalize(L + V);
            vec3 N = f_normal;
            float d = distance(lights[i].pos, f_vertex);

            float attenuation = 1.0f /
                (lights[i].attenuation.x
                + lights[i].attenuation.y * d
                + lights[i].attenuation.z * d * d);

            if (spot_enable) {
                attenuation *= pow((spot_angle - spot_limit) / (1 - spot_limit), 2);
            }

            vec3 emissive;
            if (material.emissive.tex_on) {
                emissive = vec3(texture(material.emissive.tex, f_tex_coord));
            } else {
                emissive = material.emissive.color;
            }

            color += emissive;

            vec3 ambient;
            if (material.ambient.tex_on) {
                ambient = vec3(texture(material.ambient.tex, f_tex_coord));
            } else {
                ambient = material.ambient.color;
            }

            color += ambient * material.tint * lights[i].ambient;


            if (dot(N, L) > 0) {
                vec3 diffuse;
                if (material.diffuse.tex_on) {
                    diffuse = vec3(texture(material.diffuse.tex, f_tex_coord));
                } else {
                    diffuse = material.diffuse.color;
                }

                color += diffuse * material.tint * lights[i].diffuse
                    * dot(N, L)
                    * attenuation;

                vec3 specular;
                if (material.specular.tex_on) {
                    specular = vec3(texture(material.specular.tex, f_tex_coord));
                } else {
                    specular = material.specular.color;
                }

                color += specular * lights[i].specular
                    * pow(max(dot(N, H), 0), material.shininess)
                    * attenuation;

                dither = true;
            }
        }
    }

    if (dither) {
        ivec2 noise_size = textureSize(light_noise, 0);
        float noise = texture(light_noise, gl_FragCoord.xy / noise_size).r * 4;
        // if (gl_FragCoord.x > 320) {
            color += vec3(noise - 0.5) / 48.0f;
            // color = vec3(noise);
        // }
    }

    color = clamp(color, 0, 1);

    return color;
}

void main()
{
    vec3 color = vec3(0, 0, 0);

    if (clip && dot(vec4(f_vertex, 1.0), clip_plane) < -0.01f) {
        discard;
    }

    float fog_factor = 0;

    if (fog.y > 0) {
        fog_factor = (distance(eye, f_vertex) - fog.x) / (fog.y - fog.x);
        fog_factor = clamp(fog_factor, 0.0, 1.0);
    }

    color = add_lights(color);

    output_color = vec4(color, material.alpha * (1 - fog_factor));
}
