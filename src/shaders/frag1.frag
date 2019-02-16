#version 420 core
out vec4 FragColor;
in vec3 inputColor;
in vec2 Tex;

uniform sampler2D mTex1;
uniform sampler2D mTex2;
void main()
{
FragColor = mix(texture(mTex1, Tex), texture(mTex2, Tex), 0.0);
}