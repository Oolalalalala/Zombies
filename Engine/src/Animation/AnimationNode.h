#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "Core/Buffer.h"


struct KeyPosition
{
    glm::vec3 Position;
    float TimeStamp;
};

struct KeyRotation
{
    glm::quat Rotation;
    float TimeStamp;
};

struct KeyScale
{
    glm::vec3 Scale;
    float TimeStamp;
};

// Animation for a single bone
struct AnimationNode
{
public:
    std::vector<KeyPosition> KeyPositions;
    std::vector<KeyRotation> KeyRotations;
    std::vector<KeyScale> KeyScales;

    glm::mat4 GetLocalTransform(float animationTime);

private:
    glm::vec3 InterpolatePosition(float animationTime);
    glm::quat InterpolateRotation(float animationTime);
    glm::vec3 InterpolateScale(float animationTime);
};
