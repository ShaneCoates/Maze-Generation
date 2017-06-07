#version 150

in vec2 TexCoord;

uniform sampler2D tex;

out vec4 outColor;

void main()
{
	outColor = vec4(texture(tex, TexCoord));
}