#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 result =  projection * view * model * vec4(aPos.x, aPos.y , aPos.z, 1.0);
    gl_Position = vec4(result.x, result.y, result.z, result.w);

}