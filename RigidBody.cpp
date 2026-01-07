#include "RigidBody.h"

#include "MathUtils.h"

#include "Transform.h"

//#include <iostream>

RigidBody::RigidBody(Transform* transform, bool useGravity, float cXPos, float cYPos, float cZPos, float cXSize, float cYSize, float cZSize)
	: transform(transform), useGravity(useGravity), AABBCollider(transform, cXSize, cYSize, cZSize, cXPos, cYPos, cZPos )
{
	velocity = DirectX::XMVectorSet(0, 0, 0, 0);
}

void RigidBody::TickThisRigidBodyPhysics(float deltaTime)
{
	if (useGravity) {
		velocity = DirectX::XMVectorAdd(velocity, DirectX::XMVECTOR{ 0, yGravity * deltaTime, 0, 0 });
		//velocity = DirectX::XMVectorSet(DirectX::XMVectorGetX(velocity), DirectX::XMVectorGetY(velocity) + (yGravity * deltaTime), DirectX::XMVectorGetZ(velocity), 1);
		//std::cout << "vel " << DirectX::XMVectorGetY(velocity) << std::endl;

	}

	//std::cout << "pos before " << DirectX::XMVectorGetY(pos) << std::endl;
	transform->Translate(MultFloatAndXMVector(velocity, deltaTime));
	//std::cout << "pos after " << DirectX::XMVectorGetY(transform->position) << std::endl;

}

void RigidBody::SetVelocity(DirectX::XMVECTOR newVelocity)
{
	velocity = newVelocity;
}

void RigidBody::SetVelocityX(float x)
{
	velocity = DirectX::XMVectorSetX(velocity, x);
}

void RigidBody::SetVelocityY(float y)
{
	velocity = DirectX::XMVectorSetY(velocity, y);

}

void RigidBody::SetVelocityZ(float z)
{
	velocity = DirectX::XMVectorSetZ(velocity, z);
}

void RigidBody::AddToVelocity(DirectX::XMVECTOR velocityToAdd)
{
	velocity = DirectX::XMVectorAdd(velocityToAdd, velocityToAdd);
}
