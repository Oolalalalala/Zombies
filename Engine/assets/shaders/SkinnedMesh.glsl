#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in ivec4 a_BoneIndex;
layout(location = 4) in vec4 a_BoneWeight;


out vec3 normal;
out vec2 texCoord;

layout(std140, binding = 1) uniform CameraData
{
	mat4 u_ViewProjection;
	vec3 u_CameraPosition;
};

const int MAX_BONES = 256;

layout(std140, binding = 2) uniform BoneTransform
{
	mat4 u_BoneTransforms[MAX_BONES]; // max size
};

uniform mat4 u_Model;

void main()
{
	vec4 totalPosition = vec4(0.0f);
    for(int i = 0; i < 4 ; i++)
    {
        totalPosition += u_BoneTransforms[a_BoneIndex[i]] * vec4(a_Position,1.0f) * a_BoneWeight[i];
    }

	normal = a_Normal;
	texCoord = a_TexCoord;

	gl_Position = u_ViewProjection * u_Model * totalPosition;
}

///////////////////////////////////////////////////////////////////

#type fragment
#version 450 core

out vec4 fragColor;

in vec3 normal;
in vec2 texCoord;

uniform sampler2D u_DiffuseTexture;


void main()
{
	//fragColor = vec4(normal, 1.0f);
	fragColor = texture(u_DiffuseTexture, texCoord);
	//fragColor = vec4(texCoord.x, texCoord.y, 0.0, 1.0);
}