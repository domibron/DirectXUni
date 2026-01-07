#pragma once

#include "RigidBody.h"

#include "Transform.h"

class MovingEntity
	: public RigidBody
{
public:
	MovingEntity(bool useGravity = true, float cXPos = 0, float cYPos = 0, float cZPos = 0, float cXSize = 1.0f, float cYSize = 1.0f, float cZSize = 1.0f);

	Transform transform;
};

