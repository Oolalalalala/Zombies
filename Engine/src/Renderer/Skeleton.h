#pragma once

#include "Animation/Animation.h"

struct Bone
{
	glm::mat4 InverseBindMatrix = glm::mat4(1.0f);
	std::vector<int> Children;
};

class Skeleton
{
public:
	Skeleton(const std::vector<Bone>& bones);
	void SetBones(const std::vector<Bone>& bones);

	void GetBoneTransforms(Ref<Animation> anination, float animationTime, std::vector<glm::mat4>& buffer);
	uint32_t GetBoneCount() { return m_Bones.size(); };

private:
	void FindRootBones();
	void GetBoneTransformRecursive(Ref<Animation> animation, float animationTime, const glm::mat4& parentTransform, uint32_t boneIndex, std::vector<glm::mat4>& buffer);

private:
	std::vector<Bone> m_Bones;
	std::vector<uint32_t> m_RootBoneIndices;
};