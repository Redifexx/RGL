#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal; // the texture variable has attribute position 2
  
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0f));
    Normal = mat3(transpose(inverse(model))) * aNormal; //ineeficient but good for now
    TexCoord = aTexCoord;
}    