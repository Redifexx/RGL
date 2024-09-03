#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTex;
layout (location = 2) in vec3 aNormal;

uniform mat4 modelMatrix;
uniform mat4 worldMatrix;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out float DepthPos;

void main()
{
	vec4 worldPosition = worldMatrix * modelMatrix * vec4(aPos, 1.0);
	gl_Position = projection * worldPosition;
	DepthPos = gl_Position.z;
	FragPos = worldPosition.xyz;
	Normal = normalize(mat3(transpose(inverse((worldMatrix * modelMatrix)))) * aNormal);
}