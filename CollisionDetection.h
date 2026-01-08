#pragma once

#include "DirectXMath.h"

class BlockObject;

//class Transform;


class Ray;

template<typename T>
struct RayCollision;

class CollisionDetection
{
private:
	float DotProduct(DirectX::XMVECTOR a, DirectX::XMVECTOR b);
public:
	bool RayAABBIntersection(Ray& r, BlockObject* collider, RayCollision<BlockObject>& collision);
};

