#include "pch.h"
#include "AnimationNode.h""

//#define LINEAR_SEARCH
#define STD_BINARY

glm::mat4 AnimationNode::GetLocalTransform(float animationTime)
{
	return glm::translate(glm::mat4(1.0f), InterpolatePosition(animationTime)) *
		   glm::toMat4(InterpolateRotation(animationTime))                     *
		   glm::scale(glm::mat4(1.0f), InterpolateScale(animationTime));
}

glm::vec3 AnimationNode::InterpolatePosition(float animationTime)
{
	if (KeyPositions.size() == 0)
		return glm::vec3(0.0f);
	if (KeyPositions.size() == 1)
		return KeyPositions[0].Position;


#ifdef LINEAR_SEARCH
	int idx = 1;
	while (idx < KeyPositions.size() - 1)
	{
		if (KeyPositions[idx].TimeStamp >= animationTime)
			break;

		idx++;
	}
	float t1 = KeyPositions[idx - 1].TimeStamp;
	float t2 = KeyPositions[idx].TimeStamp;
	float scaleFactor = (animationTime - t1) / (t2 - t1);

	return glm::mix(KeyPositions[idx - 1].Position, KeyPositions[idx].Position, scaleFactor);
#elif defined(STD_BINARY)

	KeyPosition searchKey;
	searchKey.TimeStamp = animationTime;
	auto it = std::upper_bound(KeyPositions.begin(), KeyPositions.end(), searchKey, [](const KeyPosition& lhs, const KeyPosition& rhs) {
		return lhs.TimeStamp < rhs.TimeStamp; });

	if (it == KeyPositions.end())
		it--;

	float t1 = (it - 1)->TimeStamp;
	float t2 = it->TimeStamp;
	float scaleFactor = (animationTime - t1) / (t2 - t1);

	return glm::mix((it - 1)->Position, it->Position, scaleFactor);
#else

	uint32_t left = 0;
	uint32_t right = KeyPositions.size() - 1;
	uint32_t mid;
	while (right - left > 1)
	{
		mid = (left + right) / 2;
		if (animationTime > KeyPositions[mid].TimeStamp)
			left = mid;
		else
			right = mid;
	}

	
	float t1 = KeyPositions[left].TimeStamp;
	float t2 = KeyPositions[right].TimeStamp;
	float scaleFactor = (animationTime - t1) / (t2 - t1);

	return glm::mix(KeyPositions[left].Position, KeyPositions[right].Position, scaleFactor);

#endif
}

glm::quat AnimationNode::InterpolateRotation(float animationTime)
{
	if (KeyRotations.size() == 0)
		return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (KeyRotations.size() == 1)
		return KeyRotations[0].Rotation;


#ifdef LINEAR_SEARCH
	int idx = 1;
	while (idx < KeyRotations.size() - 1)
	{
		if (KeyRotations[idx].TimeStamp >= animationTime)
			break;

		idx++;
	}

	float t1 = KeyRotations[idx - 1].TimeStamp;
	float t2 = KeyRotations[idx].TimeStamp;

	float scaleFactor = (animationTime - t1) / (t2 - t1);
	return glm::slerp(KeyRotations[idx - 1].Rotation, KeyRotations[idx].Rotation, scaleFactor);
	
#elif defined(STD_BINARY)
	
	KeyRotation searchKey;
	searchKey.TimeStamp = animationTime;
	auto it = std::upper_bound(KeyRotations.begin(), KeyRotations.end(), searchKey, [](const KeyRotation& lhs, const KeyRotation& rhs) {
		return lhs.TimeStamp < rhs.TimeStamp; });

	if (it == KeyRotations.end())
		it--;

	float t1 = (it - 1)->TimeStamp;
	float t2 = it->TimeStamp;
	float scaleFactor = (animationTime - t1) / (t2 - t1);

	return glm::slerp((it - 1)->Rotation, it->Rotation, scaleFactor);
#else

	uint32_t left = 0;
	uint32_t right = KeyRotations.size() - 1;
	uint32_t mid;
	while (right - left > 1)
	{
		mid = (left + right) / 2;
		if (animationTime > KeyRotations[mid].TimeStamp)
			left = mid;
		else
			right = mid;
	}

	
	float t1 = KeyRotations[left].TimeStamp;
	float t2 = KeyRotations[right].TimeStamp;
	float scaleFactor = (animationTime - t1) / (t2 - t1);

	return glm::slerp(KeyRotations[left].Rotation, KeyRotations[right].Rotation, scaleFactor);

#endif

}

glm::vec3 AnimationNode::InterpolateScale(float animationTime)
{
	if (KeyScales.size() == 0)
		return glm::vec3(1.0f);
	if (KeyScales.size() == 1)
		return KeyScales[0].Scale;

	
#ifdef LINEAR_SEARCH
	int idx = 1;
	while (idx < KeyScales.size() - 1)
	{
		if (KeyScales[idx].TimeStamp >= animationTime)
			break;

		idx++;
	}

	float t1 = KeyScales[idx - 1].TimeStamp;
	float t2 = KeyScales[idx].TimeStamp;

	float scaleFactor = (animationTime - t1) / (t2 - t1);
	return glm::mix(KeyScales[idx - 1].Scale, KeyScales[idx].Scale, scaleFactor);
	
#elif defined(STD_BINARY)
	
	KeyScale searchKey;
	searchKey.TimeStamp = animationTime;
	auto it = std::upper_bound(KeyScales.begin(), KeyScales.end(), searchKey, [](const KeyScale& lhs, const KeyScale& rhs) {
		return lhs.TimeStamp < rhs.TimeStamp; });

	if (it == KeyScales.end())
		it--;

	float t1 = (it - 1)->TimeStamp;
	float t2 = it->TimeStamp;
	float scaleFactor = (animationTime - t1) / (t2 - t1);

	return glm::mix((it - 1)->Scale, it->Scale, scaleFactor);

#else

	uint32_t left = 0;
	uint32_t right = KeyScales.size() - 1;
	uint32_t mid;
	while (right - left > 1)
	{
		mid = (left + right) / 2;
		if (animationTime > KeyScales[mid].TimeStamp)
			left = mid;
		else
			right = mid;
	}

	
	float t1 = KeyScales[left].TimeStamp;
	float t2 = KeyScales[right].TimeStamp;
	float scaleFactor = (animationTime - t1) / (t2 - t1);

	return glm::mix(KeyScales[left].Scale, KeyScales[right].Scale, scaleFactor);

#endif
}

