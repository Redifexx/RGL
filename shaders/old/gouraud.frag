#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 LightColor;

out vec4 FragColor;

void main()
{
    vec3 result = LightColor;
    FragColor = vec4(result, 1.0);
}