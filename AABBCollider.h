#pragma once

#include <DirectXMath.h>

class Transform;

//struct XMVECTOR;

struct AABBData {

	float xSize, ySize, zSize;
	float xPos, yPos, zPos;
};

class AABBCollider
{
private:
	AABBData colliderData;
	Transform* boxTrans;

	float DotProduct(DirectX::XMVECTOR a, DirectX::XMVECTOR b);

public:
	AABBData GetBounds() { return colliderData; }

	AABBCollider(Transform* transform, float xSize, float ySize, float zSize, float xPos, float yPos, float zPos);
	AABBCollider(Transform* transform, AABBData data);

	bool CheckForCollision(AABBData secondObject, DirectX::XMVECTOR secondObjectPos);
	// Checks to see how much the objects are overlapping, how much the second object is intersecting the first. (how much to move the first object to avoid the collision)
	DirectX::XMVECTOR GetOverlapAmount(AABBData secondObject, DirectX::XMVECTOR secondObjectPos);

	DirectX::XMVECTOR WhichSideIsFistCollision(AABBData secondObject, DirectX::XMVECTOR secondObjectPos);
};

