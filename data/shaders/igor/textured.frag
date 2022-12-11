#version 450

in vec2 VertexTexCoord;

layout(location = 0) out vec4 out_color;

uniform sampler2D igor_matTexture0;

void main()
{
    out_color.rgba = texture2D(igor_matTexture0, VertexTexCoord).rgba;
}
