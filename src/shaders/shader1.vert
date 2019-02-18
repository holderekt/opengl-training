#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColors;
layout (location = 1) in vec2 aTex;

out vec3 inputColor;
out vec2 Tex;
uniform float xOffset;
uniform float yOffset;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 result =  projection * view * model * vec4(aPos.x, aPos.y , aPos.z, 1.0);
    gl_Position = vec4(result.x +xOffset, result.y +yOffset, result.z, result.w);
    Tex = vec2(aTex.x + (xOffset), aTex.y + (yOffset));
}