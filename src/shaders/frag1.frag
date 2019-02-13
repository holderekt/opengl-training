#version 420 core
out vec4 FragColor;
in vec3 inputColor;
void main()
{
FragColor = vec4(inputColor, 1.0);
}