#version 430

layout(location = 0) out vec4 out_color;

uniform vec3 igor_colorID;

void main()
{
	out_color.rgb = igor_colorID;
	out_color.a = 1.0;
}
