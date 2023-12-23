#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in uint a_TexIndex;

layout(location = 0) out vec4 color;
layout(location = 1) out vec2 texCoord;
layout(location = 2) flat out uint texIndex;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
	mat4 u_OrthoProjection;
};


void main()
{
	texCoord = a_TexCoord;
	color = a_Color;
	texIndex = a_TexIndex & 0x7fffffffU;

	uint isOrtho = a_TexIndex >> 31;
	gl_Position = ((1U - isOrtho) * u_ViewProjection + isOrtho * u_OrthoProjection) * vec4(a_Position, 1.0f);
}

///////////////////////////////////////////////////////////////////

#type fragment
#version 450 core

out vec4 fragColor;

layout(location = 0) in vec4 color;
layout(location = 1) in vec2 texCoord;
layout(location = 2) flat in uint texIndex;

uniform sampler2D u_Textures[32];

void main()
{
	vec4 texColor = color * texture(u_Textures[texIndex], texCoord);
	fragColor = texColor;
}