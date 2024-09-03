#version 330 core

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;

void main()
{
    FragColor = vec4(Normal * 0.5 + 0.5, 1.0);
}