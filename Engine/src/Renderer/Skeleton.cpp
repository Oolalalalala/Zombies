#include "pch.h"
#include "Skeleton.h"


Skeleton::Skeleton(const std::vector<Bone>& bones)
{
	SetBones(bones);
}

void Skeleton::SetBones(const std::vector<Bone>& bones)
{
    m_Bones = bones;

	FindRootBones();
}

void Skeleton::GetBoneTransforms(Ref<Animation> animation, float animationTime, std::vector<glm::mat4>& buffer)
{
	CORE_VERIFY(buffer.size() >= m_Bones.size(), "Buffer not large enough to load all transform for animation");

    animationTime = fmod(animationTime, animation->Length);


	for (uint32_t rootIndex : m_RootBoneIndices)
		GetBoneTransformRecursive(animation, animationTime, glm::mat4(1.0f), rootIndex, buffer);


    // Apply inverse bind matrix
    for (uint32_t i = 0; i < m_Bones.size(); i++)
    {
        buffer[i] = buffer[i] * m_Bones[i].InverseBindMatrix;
    }
}

void Skeleton::FindRootBones()
{
    std::vector<bool> isRoot(m_Bones.size(), true);
    for (auto& bone : m_Bones)
    {
       	for (uint32_t childIdx : bone.Children)
        	isRoot[childIdx] = false;
    }
    for (int i = 0; i < isRoot.size(); i++)
    {
      	if (isRoot[i])
      		m_RootBoneIndices.emplace_back(i);
    }
     
    CORE_VERIFY(m_RootBoneIndices.size(), "No root bone is found");
}

void Skeleton::GetBoneTransformRecursive(Ref<Animation> animation, float animationTime, const glm::mat4& parentTransform, uint32_t boneIndex, std::vector<glm::mat4>& buffer)
{
    buffer[boneIndex] = parentTransform * animation->Nodes[boneIndex].GetLocalTransform(animationTime);

    for (uint32_t child : m_Bones[boneIndex].Children)
    {
		GetBoneTransformRecursive(animation, animationTime, buffer[boneIndex], child, buffer);
    }
}