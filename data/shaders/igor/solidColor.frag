#version 450

layout(location = 0) out vec4 out_color;

uniform vec4 igor_solidColor;

void main()
{
	out_color = igor_solidColor;
}
