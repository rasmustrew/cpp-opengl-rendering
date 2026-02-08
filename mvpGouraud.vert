#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{

    vec4 modelPos = model * vec4(aPos, 1.0);
    vec3 modelPos3 = vec3(modelPos);
    vec3 modelNormal = normalMatrix * aNormal;
    gl_Position = projection * view * modelPos;

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(modelNormal);
    vec3 lightDir = normalize(lightPos - modelPos3);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - modelPos3);
    vec3 reflectDir = reflect(-lightDir, norm); 

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * lightColor;  

    ourColor = (ambient + diffuse + specular) * objectColor;

    
} 