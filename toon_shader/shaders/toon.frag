#version 330 core

in float ToonCoord;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ToonRamp;
uniform sampler2D ColorTexture;
uniform vec4 Color;

uniform int IsOutline;
uniform vec4 OutlineColor;

void main()
{
    if (IsOutline == 1)
    {
        FragColor = OutlineColor;
        return;
    }

    vec4 baseColor = Color * texture(ColorTexture, TexCoord);

    vec4 toonLight = texture(ToonRamp, vec2(ToonCoord, 0.0));

    vec4 finalColor = baseColor * toonLight; 

    FragColor = vec4(finalColor.rgb, baseColor.a);
}