#pragma once

#include "MovingEntity.h"

#include "Camera.h"
#include "Transform.h"

class PlayerEntity
	: public MovingEntity
{
public:
	PlayerEntity();

	Camera playerCam{ &transform };

	Camera* GetCamera() { return &playerCam; }
};

