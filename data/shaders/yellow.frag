#version 430

in vec3 VertexWorld;
in vec3 VertexNormal;

layout(location = 0) out vec4 out_color;

uniform vec3 eyePosition;

uniform vec3 matAmbient;
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform float matShininess;
uniform vec3 matEmissive;

uniform vec3 lightOrientation;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

void main()
{
	out_color = vec4(1,1,0,1);
}
