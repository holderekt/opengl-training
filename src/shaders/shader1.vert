#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColors;

out vec3 inputColor;
uniform float xOffset;
uniform float yOffset;

void main()
{
    gl_Position = vec4((aPos.x + xOffset), (aPos.y + yOffset) , aPos.z, 1.0);
    inputColor = aColors;
    inputColor.x = inputColor.x + yOffset;
}