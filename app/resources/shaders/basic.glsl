//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment
#version 330 core
// Shader for directional and point light adapted from https://learnopengl.com/Lighting/
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

struct Material {
    sampler2D diffuse; // diffuse map
    vec3 specular;
    float shininess;
};

uniform Material material;

struct DirLight {
    vec3 direction; // direction
    vec3 diffuse; // diffuse component
    vec3 ambient; // ambient component
    vec3 specular; // specular component
};

uniform DirLight dirLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // diffuse vector
    vec3 diffVector = vec3(texture(material.diffuse, TexCoords));
    // combine results
    vec3 ambient  = light.ambient  * diffVector;
    vec3 diffuse  = light.diffuse  * diff * diffVector;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}

void main() {
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    FragColor = vec4(result, 1.0);
}
