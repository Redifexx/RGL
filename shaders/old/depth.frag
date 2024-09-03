#version 330 core

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in float DepthPos;

uniform float nearClip;
uniform float farClip;

void main()
{
    float z = FragPos.z;
    //z = gl_FragCoord.z;
    //z = DepthPos;
    float zPrime = (-2.0 / ((farClip) - (nearClip))) * ((farClip / 2.0) * z);
    //zPrime = (2.0 * z - 1.0);

    //Z vs ZPrime
    //zPrime = z;

    //Linear
    float zNormalized = ((zPrime) - (nearClip)) / ((farClip) - (nearClip));

    //Non-Linear
    //float zNormalized = ((1.0/zPrime) - (1.0/nearClip)) / ((1.0/farClip) - (1.0/nearClip));

    FragColor = vec4(vec3(zNormalized), 1.0);
}