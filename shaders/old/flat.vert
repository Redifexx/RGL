#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTex;
layout (location = 2) in vec3 aNormal;


uniform mat4 modelMatrix;
uniform mat4 worldMatrix;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
flat out vec3 LightColor;

//From LearnOpenGL
struct PointLight
{
    vec3 pos;
    vec3 color;
    float ambientStrength;
    float specularStrength;
};
#define POINT_LIGHTS 2

PointLight pointLights[POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos)
{
    //Look Vectors
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.pos - fragPos);

    //Ambient
    vec3 ambient = light.ambientStrength * light.color;

    //Diffuse
    float diff = max(0.0, dot(norm, lightDir));
    vec3 diffuse = diff * light.color;

    //Specular
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0f);
    vec3 specular = light.specularStrength * spec * light.color;

    //Atenuation
    float distance = length(light.pos - fragPos);
    float attenuation = 1.0 / (1.0 + 0.1 * distance + 0.01 * distance * distance);

    return (ambient + attenuation * (diffuse + specular));
}

void main()
{
    vec4 worldPosition = worldMatrix * modelMatrix * vec4(aPos, 1.0);
	gl_Position = projection * worldPosition;
    FragPos = worldPosition.xyz;

	Normal = normalize(mat3(transpose(inverse((worldMatrix * modelMatrix)))) * aNormal);

    vec3 objectColor = vec3(1.0f, 1.0f, 1.0f);

    PointLight red;
    red.pos = vec3(-4.0f, 0.0f, 3.0f);
    red.color = vec3(1.0f, 0.0f, 0.0f);
    red.ambientStrength = 0.1f;
    red.specularStrength = 0.75f;

    PointLight blue;
    blue.pos = vec3(4.0f, 0.0f, 3.0f);
    blue.color = vec3(0.0f, 0.0f, 1.0f);
    blue.ambientStrength = 0.1f;
    blue.specularStrength = 0.75f;

    PointLight green;
    green.pos = vec3(5.0f, 1.0f, 35.0f);
    green.color = vec3(0.0f, 1.0f, 0.0f);
    green.ambientStrength = 0.1f;
    green.specularStrength = 0.75f;

    PointLight white;
    white.pos = vec3(-5.0f, 1.0f, 35.0f);
    white.color = vec3(1.0f, 1.0f, 1.0f);
    white.ambientStrength = 0.1f;
    white.specularStrength = 0.75f;

    pointLights[0] = red;
    pointLights[1] = blue;
    //pointLights[2] = green;
    //pointLights[3] = white;


    //Only since camera is static

    vec3 result = vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < POINT_LIGHTS; i++)
    {
        result += CalcPointLight(pointLights[i], Normal, FragPos);
    }
    
    LightColor = result * objectColor;
}