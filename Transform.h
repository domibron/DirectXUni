#pragma once

#include <DirectXMath.h>

using namespace DirectX; // This is bad practice!

class Transform
{
public:
	XMVECTOR position{ 0, 0, 0 };
	XMVECTOR rotation{ 0, 0, 0 };
	XMVECTOR scale{ 1, 1, 1 };

	XMMATRIX GetWorldMatrix();
	void Translate(XMVECTOR translation);
	void Rotate(XMVECTOR inRotation);

	XMVECTOR GetForward();
	XMVECTOR GetRight();
	XMVECTOR GetUp();
};

