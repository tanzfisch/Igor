<?xml version="1.0"?>
<Igor>
	<ShaderMaterial name="igor.colorID" order="200" visibility="Private">
		<States>
			<DepthTest>On</DepthTest>
			<DepthFunc>Less</DepthFunc>
			<DepthMask>On</DepthMask>
			<Blend>Off</Blend>
			<CullFace>On</CullFace>
			<CullFaceFunc>Back</CullFaceFunc>
			<Wireframe>Off</Wireframe>
			<Instanced>Off</Instanced>
			<InstancedFunc>PositionOrientation</InstancedFunc>
		</States>
		<Program>
			<Vertex>
<![CDATA[#version 450

layout(location = 0) in vec3 in_position;

uniform mat4 igor_modelViewProjection;

void main()
{
    gl_Position = igor_modelViewProjection * vec4(in_position,1);
}]]>
			</Vertex>
			<Fragment>
<![CDATA[#version 450

layout(location = 0) out vec4 out_color;

uniform vec4 igor_solidColor;

void main()
{
	out_color = igor_solidColor;
}]]>
			</Fragment>
		</Program>
	</ShaderMaterial>
</Igor>