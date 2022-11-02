#version 430

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;

uniform mat4 igor_modelViewProjection;

out vec4 VertexColor;

void main()
{
	VertexColor = in_color;

	gl_Position = igor_modelViewProjection * vec4(in_position,1);
}
