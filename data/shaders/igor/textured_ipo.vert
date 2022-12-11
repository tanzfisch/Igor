#version 450

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texCoord;

layout(location = 3) in mat4 in_modelMatrix;

uniform mat4 igor_modelViewProjection;

out vec3 VertexWorld;
out vec3 VertexNormal;
out vec2 VertexTexCoord;

void main()
{
	VertexWorld = (in_modelMatrix * vec4(in_position, 1)).xyz;
	
	mat4 norm = transpose(inverse(in_modelMatrix));
	VertexNormal = (norm * vec4(in_normal, 1)).xyz;
	
	VertexTexCoord = in_texCoord;
	
	gl_Position = igor_modelViewProjection * in_modelMatrix * vec4(in_position, 1);
}
