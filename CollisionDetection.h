#pragma once

#include "DirectXMath.h"

class BlockObject;

class AABBCollider;
//class Transform;


class Ray;

template<typename T>
struct RayCollision;

class CollisionDetection
{
public:
	bool RayAABBIntersection(Ray& r, DirectX::XMVECTOR& pos, AABBCollider& collider, RayCollision<BlockObject>& collision);
};

