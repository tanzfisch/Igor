#version 430

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

uniform mat4 mvp_matrix;
uniform mat4 model_matrix;

out vec3 VertexWorld;
out vec3 VertexNormal;

void main()
{
	VertexWorld = (model_matrix * vec4(in_position, 1)).xyz;

	mat4 norm = transpose(inverse(model_matrix));
	VertexNormal = (norm * vec4(in_normal, 1)).xzy;	
	
	gl_Position = mvp_matrix * vec4(in_position,1);
}
