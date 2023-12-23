#include "pch.h"
#include "Animation.h"

Animation::Animation(const std::vector<AnimationNode> nodes, const std::string& name, float length)
{
	Nodes = nodes;
	Name = name;
	Length = length;
}
