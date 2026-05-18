#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out float ToonCoord;

uniform mat4 WorldMatrix;
uniform mat4 ViewProjMatrix;
uniform vec3 EyePosition;

void main()
{
    vec3 worldPosition = (WorldMatrix * vec4(VertexPosition, 1.0)).xyz;

    mat3 normalMatrix = transpose(inverse(mat3(WorldMatrix)));
    vec3 worldNormal = normalize(normalMatrix * VertexNormal);

    vec3 viewDir = normalize(EyePosition - worldPosition);

    ToonCoord = clamp(dot(worldNormal, viewDir), 0.0, 1.0);

    gl_Position = ViewProjMatrix * vec4(worldPosition, 1.0);
}