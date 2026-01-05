#pragma once

#include <DirectXMath.h>

//struct XMVECTOR;

struct AABBData {

	float xSize, ySize, zSize;
	float xPos, yPos, zPos;
};

class AABBCollider
{
private:
	AABBData colliderData;

public:
	AABBCollider(float xSize, float ySize, float zSize, float xPos, float yPos, float zPos);
	AABBCollider(AABBData data);

	static bool CheckForCollision(AABBData firstObject, DirectX::XMVECTOR firstObjectPosition, AABBData secondObject, DirectX::XMVECTOR secondObjectPos);
	// Checks to see how much the objects are overlapping, how much the second object is intersecting the first. (how much to move the first object to avoid the collision)
	static DirectX::XMVECTOR GetOverlapAmount(AABBData firstObject, DirectX::XMVECTOR firstObjectPosition, AABBData secondObject, DirectX::XMVECTOR secondObjectPos);
};

