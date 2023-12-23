#type vertex
#version 450 core


layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec4 a_Outline; // 3 float for color, 1 float for border width
layout(location = 3) in vec2 a_TexCoord;
layout(location = 4) in int a_TexIndex;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 outline;
layout(location = 2) out vec2 texCoord;
layout(location = 3) flat out int texIndex;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
	mat4 u_OrthoProjection;
};

void main()
{
	texCoord = a_TexCoord;
	color = a_Color;
	outline = a_Outline;
	texIndex = a_TexIndex;
	
	uint isOrtho = a_TexIndex >> 31;
	gl_Position = ((1U - isOrtho) * u_ViewProjection + isOrtho * u_OrthoProjection) * vec4(a_Position, 1.0f);
}

///////////////////////////////////////////////////////////////////

#type fragment
#version 450 core


layout(location = 0) in vec4 color;
layout(location = 1) in vec4 outline;
layout(location = 2) in vec2 texCoord;
layout(location = 3) flat in int texIndex;

out vec4 fragColor;

uniform sampler2D u_Textures[16];


void main()
{
	float outlineWidth = outline.a;

	float d = texture(u_Textures[texIndex], texCoord).r;
	float aaf = fwidth(d);
	float alpha = smoothstep(0.5 - outlineWidth - aaf, 0.5 - outlineWidth + aaf, d);
	float border = smoothstep(0.5 - aaf, 0.5 + aaf, d);
	
	fragColor = vec4(mix(outline.rgb, color.rgb, border), alpha * color.a);

}