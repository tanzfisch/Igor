<?xml version="1.0"?>
<Igor>
	<!-- material definition -->
	<ShaderMaterial name="mica.emitterOverlay.flat" zIndex="MAX" visibility="Private">
		<States>
            <DepthMask>Off</DepthMask>
            <Blend>On</Blend>
            <CullFace>Off</CullFace>
		</States>
        <Program>
            <Vertex>
<![CDATA[#version 450
 
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
}]]>
            </Vertex>
            <Fragment>
<![CDATA[#version 450 
 
in vec3 VertexWorld; 
in vec3 VertexNormal; 
 
layout(location = 0) out vec4 out_color; 
 
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
	 
	vec3 emissive = igor_matEmissive; 
	 
	// Compute the ambient term 
	vec3 ambient = igor_matAmbient * igor_lightAmbient; 
	 
	// Compute the diffuse term 
	vec3 L = normalize(igor_lightOrientation); 
	float diffuseLightFactor = max(dot(N, L), 0.0); 
	vec3 diffuse = igor_matDiffuse * igor_lightDiffuse * diffuseLightFactor; 
	 
	// Compute the specular term 
	vec3 V = normalize(igor_eyePosition - P); 
	vec3 H = normalize(L + V); 
	float specularLightFactor = pow(max(dot(N, H), 0.0), igor_matShininess); 
	 
	if (igor_matShininess <= 0.0)  
	{ 
		specularLightFactor = 0.0; 
	} 
	 
	vec3 specular = igor_matSpecular * igor_lightSpecular * specularLightFactor; 
	 
	out_color.rgb = emissive + ambient + diffuse + specular; 
	out_color.a = igor_matAlpha; 
}]]>l
            </Fragment>
        </Program>            
	</ShaderMaterial>
</Igor>