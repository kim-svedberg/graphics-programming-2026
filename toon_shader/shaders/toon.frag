#version 330 core

in vec3 WorldPosition;
in vec3 WorldNormal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ToonRamp;
uniform vec3 EyePosition;

void main()
{
    vec3 N = normalize(WorldNormal);
    vec3 V = normalize(EyePosition - WorldPosition);

    float intensity = dot(N, V);

    intensity = clamp(intensity, 0.0, 1.0);

    FragColor = texture(ToonRamp, vec2(intensity, 0.0));
}