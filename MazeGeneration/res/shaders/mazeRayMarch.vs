#version 150

in vec2 position;
in vec2 texcoord;

smooth out vec2 TexCoord;

void main()
{
	TexCoord = position;
	gl_Position = vec4(position, 0.0, 1.0);
}