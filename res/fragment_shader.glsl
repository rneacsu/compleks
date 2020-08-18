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

struct material_t {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D ambient;
    sampler2D emissive;
    float shininess;
};

uniform light_t lights[MAX_LIGHTS];
uniform sampler2D light_noise;

uniform material_t material;

uniform vec3 eye;

in vec3 f_vertex;
in vec3 f_normal;
in vec2 f_tex_coord;

out vec4 output_color;

vec3 add_light(vec3 color)
{
    vec3 V = normalize(eye - f_vertex);
    bool dither = false;

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
            
            color += vec3(texture(material.emissive, f_tex_coord));
            color += vec3(texture(material.ambient, f_tex_coord)) 
                * lights[i].ambient;

            if (dot(N, L) > 0) {
                color += vec3(texture(material.diffuse, f_tex_coord)) 
                    * lights[i].diffuse 
                    * dot(N, L) 
                    * attenuation;
                color += vec3(texture(material.specular, f_tex_coord))  
                    * lights[i].specular 
                    * pow(max(dot(N, H), 0), material.shininess) 
                    * attenuation;
                dither = true;
            }
        }
    }

    if (dither) {
        ivec2 noise_size = textureSize(light_noise, 0);
        float noise = texture(light_noise, gl_FragCoord.xy / noise_size).r;
        // if (gl_FragCoord.x > 320) {
            color += mix(-0.5/255.0, 0.5/255.0, noise);
            // color = vec3(noise);
        // }
    }

    color = clamp(color, 0, 1);

    return color;
}

void main()
{
    vec3 color = vec3(0, 0, 0);

    color = add_light(color);

    output_color = vec4(color, 1.0f);
}
