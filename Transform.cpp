#include "Transform.h"

XMMATRIX Transform::GetWorldMatrix()
{
	// Must follow SRT scale, rotation, transformation when combining matrices.

	XMMATRIX scaleMat = XMMatrixScalingFromVector(scale);
	XMMATRIX rotationMat = XMMatrixRotationRollPitchYawFromVector(rotation);
	XMMATRIX translationMat = XMMatrixTranslationFromVector(position);
	return scaleMat * rotationMat * translationMat;

	// If instead you used XMFLOAT3s instead of XMVECTORs for your position, rotation and scale, 
	// you can use the alternative functions XMMatrixTranslation, XMMatrixRotationRollPitchYaw…

	// You can use XMConvertToRadians for rotation if you prefer to use degrees in your transforms.
}

void Transform::Translate(XMVECTOR translation)
{
	position = XMVectorAdd(position, translation);
}

void Transform::Rotate(XMVECTOR inRotation)
{
	rotation = XMVectorAddAngles(rotation, inRotation);
}

XMVECTOR Transform::GetForward()
{
	float pitch = XMVectorGetX(rotation);
	float yaw = XMVectorGetY(rotation);
	XMVECTOR direction{
		cosf(pitch) * sinf(yaw), // X
		sinf(pitch),			 // Y
		cosf(pitch) * cosf(yaw), // Z
		0.0f
	};

	return XMVector3Normalize(direction);
}

XMVECTOR Transform::GetRight()
{
	float pitch = XMVectorGetX(rotation);
	float yaw = XMVectorGetY(rotation);
	float roll = XMVectorGetZ(rotation);

	XMVECTOR direction{
		cosf(roll) * cosf(yaw) + sinf(roll) * sinf(pitch) * sinf(yaw),  // X
		sinf(roll) * cosf(pitch),										// Y
		cosf(roll) * -sinf(yaw) + sinf(roll) * sinf(pitch) * cosf(yaw), // Z
		0.0f
	};

	// This works without camera roll
	// XMVECTOR direction{
	// sin(yaw + XM_PIDIV2), // X
	// 0.0f,				 // Y
	// cos(yaw + XM_PIDIV2), // Z
	// 0.0f
	//};

	return XMVector3Normalize(direction);
}

XMVECTOR Transform::GetUp()
{
	XMVECTOR cross = XMVector3Cross(GetForward(), GetRight());

	return XMVector3Normalize(cross);
}
