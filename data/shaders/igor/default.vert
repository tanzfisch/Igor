#version 430

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;	

uniform mat4 igor_modelViewProjection;
uniform mat4 igor_model;

out vec3 VertexWorld;
out vec3 VertexNormal;

void main()
{
	VertexWorld = (igor_model * vec4(in_position, 1)).xyz;

	mat4 norm = transpose(inverse(igor_model));
	VertexNormal = (norm * vec4(in_normal, 1)).xyz;	
	
	gl_Position = igor_modelViewProjection * vec4(in_position,1);
}
