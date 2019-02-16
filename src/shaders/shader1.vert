#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColors;
layout (location = 2) in vec2 aTex;

out vec3 inputColor;
out vec2 Tex;
uniform float xOffset;
uniform float yOffset;

void main()
{
    gl_Position = vec4((aPos.x + xOffset), (aPos.y + yOffset) , aPos.z, 1.0);
    inputColor = aColors;
    Tex = vec2(aTex.x - (xOffset), aTex.y + (yOffset));
}