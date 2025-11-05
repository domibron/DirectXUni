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
