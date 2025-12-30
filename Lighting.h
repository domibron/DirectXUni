#pragma once

#include <DirectXMath.h>

#define MAX_POINT_LIGHTS 8

struct PointLight {
	DirectX::XMVECTOR position{ 0,0,0 }; // 16 bytes
	DirectX::XMVECTOR color{ 1,1,1 }; // 16 bytes

	float strength = 10; // 4 bytes
	bool enabled = false; // 4 bytes
	float padding[2]; // 8 bytes
	// 48 total bytes
};

struct DirectionalLight {
	DirectX::XMVECTOR directionFrom{ 0,0,0 };
	DirectX::XMVECTOR color{ 1,1,1 };
};