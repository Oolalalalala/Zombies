#pragma once

#include "AnimationNode.h"
#include "Core/Buffer.h"


struct AnimationNodeData
{
	Buffer<KeyPosition> KeyPositions;
	Buffer<KeyRotation> KeyRotations;
	Buffer<KeyScale> KeyScales;
};

struct Animation
{
	std::vector<AnimationNode> Nodes;
	std::vector<AnimationNode> SubmeshNodes;
	std::string Name = "";
	float Length = 0.0f;

	Animation() = default;
	Animation(const std::vector<AnimationNode> nodes, const std::string& name, float length);
};