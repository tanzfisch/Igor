#version 430

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec2 in_texCoord;
layout(location = 3) in float in_texUnit;

uniform mat4 igor_modelViewProjection;

out vec4 VertexColor;
out vec2 TexCoord;
out float TexUnit;

void main()
{
	VertexColor = in_color;
	TexCoord = in_texCoord;
	TexUnit = in_texUnit;

	gl_Position = igor_modelViewProjection * vec4(in_position,1);
}
