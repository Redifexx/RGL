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
    float specularFactor;
    float emissiveFactor;
    float shininess;
};

uniform Material material;
uniform vec3 viewPos;

//LIGHT DEFINITION
struct PointLight
{
    vec3 position;
    float intensity;
    vec3 color;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
    float intensity;
};

struct SpotLight
{
    vec3 position;
    vec3 color;
    vec3 direction;
    float intensity;
    float innerCutOff;
    float outerCutOff;
};
uniform SpotLight spotLight;

vec3 PointLightResult(PointLight light, Material material)
{

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = (diff * vec3(texture(material.diffuse, TexCoord))) * light.color;

    // Specular (Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = (light.color * vec3(material.specularFactor)) * (spec * vec3(texture(material.specularMap, TexCoord)));

    vec3 emissive = (light.color * vec3(material.emissiveFactor)) * vec3(texture(material.emissionMap, TexCoord));

    // Light Falloff
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (1.0 + 0.1 * distance + 0.01 * distance * distance); //possible optimazation

    vec3 result = (attenuation * light.intensity * (diffuse + specular + emissive));

    return result;
}

vec3 DirectionalLightResult(DirectionalLight light, Material material)
{

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = (diff * vec3(texture(material.diffuse, TexCoord))) * light.color;

    // Specular (Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = (light.color * vec3(material.specularFactor)) * (spec * vec3(texture(material.specularMap, TexCoord)));

    vec3 emissive = (light.color * vec3(material.emissiveFactor)) * vec3(texture(material.emissionMap, TexCoord));

    vec3 result = (light.intensity * (diffuse + specular + emissive));

    return result;
}

vec3 SpotLightResult(SpotLight light, Material material)
{

    //Theta and Light Dir
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    vec3 result = vec3(0.0f);
    //Diffuse
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = (diff * vec3(texture(material.diffuse, TexCoord))) * light.color;

    // Specular (Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = (light.color * vec3(material.specularFactor)) * (spec * vec3(texture(material.specularMap, TexCoord)));

    vec3 emissive = (light.color * vec3(material.emissiveFactor)) * vec3(texture(material.emissionMap, TexCoord));

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Light Falloff
    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (1.0f + 0.09f * distance + 0.032f * (distance * distance)); //possible optimazation

    //Softness intensity
    diffuse *= intensity;
    specular *= intensity;

    result = (attenuation * light.intensity * (diffuse + specular + emissive));

    return result;
}


//uniform PointLight ptLight;
//uniform DirectionalLight dirLight;

void main()
{
    // Ambient
    vec3 ambient = vec3(1.0f) * 0.1f * vec3(texture(material.diffuse, TexCoord));

    PointLight ptLight;
    ptLight.position = vec3(8.0f, 1.0f, 8.0f);
    ptLight.color = vec3(1.0f, 0.0f, 1.0f);
    ptLight.intensity = 1.0f;

    DirectionalLight dirLight;
    dirLight.direction = vec3(-1.0f, -1.0f, -1.0f);
    dirLight.color = vec3(1.0f);
    dirLight.intensity = 1.0f;

    //SpotLight spotLight;
    //spotLight.position = vec3(5.0f, 1.0f, 5.0f);
    //spotLight.direction = vec3(-0.5f, -0.5f, -0.5f);
    //spotLight.color = vec3(1.0f, 1.0f, 1.0f);
    //spotLight.intensity = 1.0f;
    //spotLight.innerCutOff = innerCutOff_;
    //spotLight.outerCutOff = outerCutOff_;


    vec3 result = vec3(0.0f);
    //result += PointLightResult(ptLight, material);
    result += DirectionalLightResult(dirLight, material);
    //result += SpotLightResult(spotLight, material);
    result += ambient;

    FragColor = vec4(result, 1.0f);
}