#version 450

in vec3 VertexWorld;
in vec3 VertexNormal;
in vec2 VertexTexCoord;

layout(location = 0) out vec4 out_color;

uniform sampler2D igor_matTexture0;

uniform vec3 igor_eyePosition;

uniform vec3 igor_matAmbient;
uniform vec3 igor_matDiffuse;
uniform vec3 igor_matSpecular;
uniform vec3 igor_matEmissive;
uniform float igor_matShininess;
uniform float igor_matAlpha;

uniform vec3 igor_lightOrientation;
uniform vec3 igor_lightAmbient;
uniform vec3 igor_lightDiffuse;
uniform vec3 igor_lightSpecular;

void main()
{
	vec3 N = normalize(VertexNormal);
	vec3 P = VertexWorld;
	vec4 textureColor = texture2D(igor_matTexture0, VertexTexCoord);
	vec3 diffuseTextureColor = textureColor.rgb;
		
	// Compute the ambient term
	vec3 ambient = igor_matAmbient * igor_lightAmbient * diffuseTextureColor;
	
	// Compute the diffuse term
	vec3 L = normalize(igor_lightOrientation);
	float diffuseLightFactor = max(dot(N, L), 0.0);
	vec3 diffuse = igor_matDiffuse * igor_lightDiffuse * diffuseLightFactor * diffuseTextureColor;
	
	// Compute the specular term
	vec3 V = normalize(igor_eyePosition - P);
	vec3 H = normalize(L + V);
	float specularLightFactor = pow(max(dot(N, H), 0.0), igor_matShininess);
	
	if (igor_matShininess <= 0.0) 
	{
		specularLightFactor = 0.0;
	}
	
	vec3 specular = igor_matSpecular * igor_lightSpecular * specularLightFactor;
	
	out_color.rgb = igor_matEmissive + ambient + diffuse + specular;
	out_color.a = textureColor.a * igor_matAlpha;
}
