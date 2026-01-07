#pragma once

#include <DirectXMath.h>
#include "Transform.h"

// this may not work, just a future note.
struct DirectX::XMMATRIX;

class Camera
{
public:
	Camera(Transform* transform);

	Transform* camTransform;

	float fov = 65;
	float nearClippingPlane = 0.1f;
	float farClippingPlane = 100.0f;

	DirectX::XMMATRIX GetViewMatrix();

	DirectX::XMMATRIX GetProjectionMatrix(int screenWidth, int screenHeight);
};

