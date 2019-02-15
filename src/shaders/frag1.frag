#version 420 core
out vec4 FragColor;
in vec3 inputColor;
in vec2 Tex;

uniform sampler2D mTex;

void main()
{
FragColor = texture(mTex, Tex);
}