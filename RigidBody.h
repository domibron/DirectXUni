#pragma once
#include "DirectXMath.h"

#include "AABBCollider.h"

class Transform;

class RigidBody
	: public AABBCollider
{
private:
	DirectX::XMVECTOR velocity;
	float yGravity = -9.81f * 0.5f;
	Transform* transform;
	float drag = 0.2f;

public:
	RigidBody(Transform* transform, bool useGravity = true, float cXPos = 0, float cYPos = 0, float cZPos = 0, float cXSize = 1, float cYSize = 1, float cZSize = 1);

	bool useGravity;

	virtual void TickThisRigidBodyPhysics(float deltaTime); // move the object forward so we can then check for collisions.

	DirectX::XMVECTOR GetVelocity() { return velocity; };


	void SetVelocity(DirectX::XMVECTOR newVelocity);
	void SetVelocityX(float x);
	void SetVelocityY(float y);
	void SetVelocityZ(float z);
	void AddToVelocity(DirectX::XMVECTOR velocityToAdd);
	
};

