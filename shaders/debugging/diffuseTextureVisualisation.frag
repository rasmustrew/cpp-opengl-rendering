#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

#define MAX_TEXTURES 64

struct Material {
    sampler2D diffuse[MAX_TEXTURES];
    sampler2D specular[MAX_TEXTURES];
    int numDiffuseTextures;
    int numSpecularTextures;
};

uniform vec3 viewPos;
uniform Material material;
uniform float shininess;


vec3 getCombinedDiffuse()
{
    // Fallback if none set (avoid division by zero)
    if (material.numDiffuseTextures <= 0) {
        return vec3(1.0); // white
    }

    vec3 color = vec3(0.0);
    for (int i = 0; i < material.numDiffuseTextures; ++i) {
        color += vec3(texture(material.diffuse[i], TexCoords));
    }
    return color / float(material.numDiffuseTextures);
}



void main()
{

    vec3 result = getCombinedDiffuse();
    
    FragColor = vec4(result, 1.0);
}