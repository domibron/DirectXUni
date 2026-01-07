#include "RigidBody.h"

#include "MathUtils.h"

#include "Transform.h"

#include <iostream>

RigidBody::RigidBody(Transform* transform, bool useGravity, float cXPos, float cYPos, float cZPos, float cXSize, float cYSize, float cZSize)
	: transform(transform), useGravity(useGravity), AABBCollider(transform, cXPos, cYPos, cZPos, cXSize, cYSize, cZSize)
{
	velocity = DirectX::XMVectorSet(0, 0, 0, 0);
}

void RigidBody::TickThisRigidBodyPhysics(float deltaTime)
{
	if (useGravity) {
		velocity = DirectX::XMVectorSet(DirectX::XMVectorGetX(velocity), DirectX::XMVectorGetY(velocity) + (yGravity * deltaTime), DirectX::XMVectorGetZ(velocity), 1);
		std::cout << "vel " << DirectX::XMVectorGetY(velocity) << std::endl;

	}

	DirectX::XMVECTOR pos = transform->position;
	std::cout << "pos before " << DirectX::XMVectorGetY(pos) << std::endl;
	transform->position = AddTwoXMVectors(pos, MultFloatAndXMVector(velocity, deltaTime));
	std::cout << "pos after " << DirectX::XMVectorGetY(transform->position) << std::endl;

}
