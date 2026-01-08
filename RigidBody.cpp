#include "RigidBody.h"

#include "MathUtils.h"

#include "Transform.h"

#include <cmath>

//#include <iostream>

float Mag(DirectX::XMVECTOR a);

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

	if (Mag(velocity) > drag * deltaTime) {
		velocity = DirectX::XMVectorAdd(velocity, DirectX::XMVectorMultiply(DirectX::XMVector3Normalize(DirectX::XMVectorSet(-DirectX::XMVectorGetX(velocity),
			-DirectX::XMVectorGetY(velocity), -DirectX::XMVectorGetZ(velocity), 1)), DirectX::XMVectorSet(drag * deltaTime, drag * deltaTime, drag * deltaTime, 1)));
	}

	if (Mag(velocity) < 0.001f) {
		velocity = DirectX::XMVECTOR{0,0,0,0};
		return;
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

float Mag(DirectX::XMVECTOR a)
{
	float x, y, z;
	x = DirectX::XMVectorGetX(a);
	y = DirectX::XMVectorGetY(a);
	z = DirectX::XMVectorGetZ(a);

	return sqrt((x * x) + (y * y) + z * z);
}
