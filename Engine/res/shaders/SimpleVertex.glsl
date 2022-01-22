#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;

out vec4 a_Color;

void main()
{
	a_Color = color;
	gl_Position = vec4(pos, 1.0f);
}
