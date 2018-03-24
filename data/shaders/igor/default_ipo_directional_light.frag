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
	vec3 N = normalize(VertexNormal);
	vec3 P = VertexWorld;
	
	vec3 emissive = matEmissive;
	
	// Compute the ambient term
	vec3 ambient = matAmbient * lightAmbient;
	
	// Compute the diffuse term
	vec3 L = normalize(lightOrientation);
	float diffuseLightFactor = max(dot(N, L), 0.0);
	vec3 diffuse = matDiffuse * lightDiffuse * diffuseLightFactor;
	
	// Compute the specular term
	vec3 V = normalize(eyePosition - P);
	vec3 H = normalize(L + V);
	float specularLightFactor = pow(max(dot(N, H), 0.0), matShininess);
	
	if (matShininess <= 0.0) 
	{
		specularLightFactor = 0.0;
	}
	
	vec3 specular = matSpecular * lightSpecular * specularLightFactor;
	
	out_color.rgb = emissive + ambient + diffuse + specular;
	out_color.a = 1;
}
