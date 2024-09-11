#version 330 core
out vec4 FragColor;  
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// MATERIAL DEFINITION
struct Material 
{
    sampler2D diffuse;
    sampler2D specularMap;
    sampler2D emissionMap;
    vec3 specular;
    float shininess;
};

//LIGHT DEFINITION
struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionalLight
{
    vec3 direction;
    
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
};


uniform Light light;
uniform Material material;
uniform vec3 viewPos;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = (diff * vec3(texture(material.diffuse, TexCoord))) * light.diffuse;

    // Specular (Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = vec3(0.5f) * (spec * vec3(texture(material.specularMap, TexCoord)));

    vec3 emissive = vec3(0.8f, 1.0f, 0.8f) * vec3(texture(material.emissionMap, TexCoord));

    //attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (1.0 + 0.1 * distance + 0.01 * distance * distance);
    float intensity = 2.0f;

    vec3 result = (ambient + attenuation * intensity * (diffuse + specular + emissive));
    FragColor = vec4(result, 1.0f);
}