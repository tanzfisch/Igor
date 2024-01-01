<?xml version="1.0"?>
<Igor>
	<!-- material definition -->
	<ShaderMaterial name="igor.skybox" zIndex="MIN" visibility="Public">
		<States>
			<DepthTest>Off</DepthTest>
            <DepthFunc>Less</DepthFunc>
            <Blend>Off</Blend>
            <CullFace>On</CullFace>
            <CullFaceFunc>Back</CullFaceFunc>
		</States>
        <Program>
            <Vertex>
<![CDATA[#version 450

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texCoord;

uniform mat4 igor_modelViewProjection;

out vec2 VertexTexCoord;

void main()
{
	VertexTexCoord = in_texCoord;
	
	gl_Position = igor_modelViewProjection * vec4(in_position,1);
}]]>
            </Vertex>
            <Fragment>
<![CDATA[#version 450

in vec2 VertexTexCoord;

layout(location = 0) out vec4 out_color;

uniform sampler2D igor_matTexture0;

void main()
{
    out_color.rgba = texture2D(igor_matTexture0, VertexTexCoord).rgba;
}
]]>
            </Fragment>
        </Program>            
	</ShaderMaterial>
</Igor>
