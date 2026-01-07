#pragma once

#include "DirectXMath.h"



class Ray
{
public:
	Ray(DirectX::XMVECTOR pos, DirectX::XMVECTOR dir);

	DirectX::XMVECTOR GetPosition() { return position; }
	DirectX::XMVECTOR GetDirection() { return direction; }

	
protected:
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR direction;

};

template<typename T>
struct RayCollision {
	T* node;
	DirectX::XMVECTOR collidedAt;
	float rayDistance;
	RayCollision(T* node, DirectX::XMVECTOR collidedAt, float distance = 0) {
		this->node = node;
		this->collidedAt = collidedAt;
		rayDistance = distance;
	}
	RayCollision() {
		node = nullptr;
	}
};

