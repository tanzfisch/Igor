#version 430

in vec3 VertexWorld;
in vec3 VertexNormal;

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
	
	vec3 texSelector = vec3(N.x*N.x, N.y*N.y, N.z*N.z);
		
	float scale = 0.02;
	float detailScale = 0.2;
	
	vec3 diffuseTextureColor = texture2D(igor_matTexture0, P.xz * scale).rgb * texSelector.y;	
	diffuseTextureColor += texture2D(igor_matTexture0, P.yz * scale).rgb * texSelector.x; 
	diffuseTextureColor += texture2D(igor_matTexture0, P.xy * scale).rgb * texSelector.z;
	vec3 detailTextureColor = texture2D(igor_matTexture0, P.xz * detailScale).rgb * texSelector.y;
	detailTextureColor += texture2D(igor_matTexture0, P.yz * detailScale).rgb * texSelector.x; 
	detailTextureColor += texture2D(igor_matTexture0, P.xy * detailScale).rgb * texSelector.z;
	
	diffuseTextureColor *= 0.5;
	detailTextureColor *= 0.5;
	diffuseTextureColor += detailTextureColor;
	
	vec3 emissive = igor_matEmissive;
	
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
	
	vec3 specular = igor_matSpecular * igor_lightSpecular * specularLightFactor;
	
	out_color.rgb = emissive + ambient + diffuse + specular;
	out_color.a = igor_matAlpha;
}
