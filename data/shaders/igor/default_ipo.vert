#version 430

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

layout(location = 3) in mat4 in_modelMatrix;

uniform mat4 mvp_matrix;

out vec3 VertexWorld;
out vec3 VertexNormal;

void main()
{
	VertexWorld = (in_modelMatrix * vec4(in_position, 1)).xyz;
	
	mat4 norm = transpose(inverse(in_modelMatrix));
	VertexNormal = (norm * vec4(in_normal, 1)).xyz;
	
	gl_Position = mvp_matrix * in_modelMatrix * vec4(in_position, 1);
}