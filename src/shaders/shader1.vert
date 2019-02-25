#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

 
out vec3 FragPos;

void main()
{
    vec4 result =  projection * view * model * vec4(aPos.x, aPos.y , aPos.z, 1.0);
    gl_Position = vec4(result.x, result.y, result.z, result.w);
    Normal = aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));

}