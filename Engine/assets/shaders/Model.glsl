#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
//layout(location = 3) in float a_TexIndex;

out vec3 normal;
out vec2 texCoord;

layout(std140, binding = 1) uniform CameraData
{
	mat4 u_ViewProjection;
	vec3 u_CameraPosition;
};

uniform mat4 u_Model;

void main()
{
	normal = a_Normal;
	texCoord = a_TexCoord;

	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0f);
}

///////////////////////////////////////////////////////////////////

#type fragment
#version 450 core

out vec4 fragColor;

in vec3 normal;
in vec2 texCoord;

uniform sampler2D u_DiffuseTexture;
uniform vec4 u_Color;


void main()
{
	//fragColor = vec4(normal, 1.0f);
	fragColor = texture(u_DiffuseTexture, texCoord) * u_Color;
	//fragColor = vec4(texCoord.x, texCoord.y, 0.0, 1.0);
}