#version 430

in vec4 VertexColor;

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = VertexColor;
}