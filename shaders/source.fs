#version 330 core
out vec4 FragColor;  
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// MATERIAL DEFINITION
struct Material 
{
    vec3 ambient;
    vec3 diffuse;
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

uniform sampler2D ourTexture;
uniform Light light;
uniform Material material;
uniform vec3 viewPos;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    // Specular (Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = vec3(0.5f) * (spec * material.specular);



    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result * texture(ourTexture, TexCoord).rgb, 1.0f);
}