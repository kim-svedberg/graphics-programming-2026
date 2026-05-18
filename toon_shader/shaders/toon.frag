#version 330 core

in float ToonCoord;

out vec4 FragColor;

uniform sampler2D ToonRamp;

void main()
{
    FragColor = texture(ToonRamp, vec2(ToonCoord, 0.0));
}