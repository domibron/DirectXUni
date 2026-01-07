#pragma once

#ifndef MATHUTILS
#define MATHUTILS

#include <DirectXMath.h>

// I could use facy defines and stuff to auto delacare and all that glory but I lack the skills :c.


DirectX::XMVECTOR AddTwoXMVectors(DirectX::XMVECTOR a, DirectX::XMVECTOR b) {
	using namespace DirectX;
	return XMVectorSet(XMVectorGetX(a) + XMVectorGetX(b), XMVectorGetY(a) + XMVectorGetY(b), XMVectorGetZ(a) + XMVectorGetZ(b), XMVectorGetW(a) + XMVectorGetW(b));
}



// Additon

DirectX::XMVECTOR AddFloatToXMVector_X(DirectX::XMVECTOR a, float x) {
	using namespace DirectX;
	return XMVectorSet(XMVectorGetX(a) + x, XMVectorGetY(a), XMVectorGetZ(a) , XMVectorGetW(a));
}

DirectX::XMVECTOR AddFloatToXMVector_Y(DirectX::XMVECTOR a, float y) {
	using namespace DirectX;
	return XMVectorSet(XMVectorGetX(a), XMVectorGetY(a) + y, XMVectorGetZ(a), XMVectorGetW(a));
}

DirectX::XMVECTOR AddFloatToXMVector_Z(DirectX::XMVECTOR a, float z) {
	using namespace DirectX;
	return XMVectorSet(XMVectorGetX(a), XMVectorGetY(a), XMVectorGetZ(a) + z, XMVectorGetW(a));
}


// Subtraction

DirectX::XMVECTOR SubFloatToXMVector_X(DirectX::XMVECTOR a, float x) {
	using namespace DirectX;
	return AddFloatToXMVector_X(a, -x);
}

DirectX::XMVECTOR SubFloatToXMVector_Y(DirectX::XMVECTOR a, float y) {
	using namespace DirectX;
	return AddFloatToXMVector_Y(a, -y);
}

DirectX::XMVECTOR SubFloatToXMVector_Z(DirectX::XMVECTOR a, float z) {
	using namespace DirectX;
	return AddFloatToXMVector_Z(a, -z);
}


// Multiply

DirectX::XMVECTOR MultFloatAndXMVector(DirectX::XMVECTOR a, float b) {
	using namespace DirectX;
	return XMVectorSet(XMVectorGetX(a) * b, XMVectorGetY(a) * b, XMVectorGetZ(a) * b, 0); // last is 0 to avoid any errors with 0 in math like divide by.
}


// Divide

DirectX::XMVECTOR DivFloatAndXMVector(DirectX::XMVECTOR a, float b) {
	using namespace DirectX;
	return XMVectorSet(XMVectorGetX(a) / b, XMVectorGetY(a) / b, XMVectorGetZ(a) / b, 0); // last is 0 to avoid any errors with 0 in math like divide by.
}


// Creators or Factories?

DirectX::XMVECTOR CreateXMVector_XYZW(float x, float y, float z, float w) {
	using namespace DirectX;
	return XMVectorSet(x,y,z,w);
}

DirectX::XMVECTOR CreateXMVector_XYZ(float x, float y, float z) {
	using namespace DirectX;
	return XMVectorSet(x, y, z, 0); // last is 0 to avoid any errors with 0 in math like divide by.
}

DirectX::XMVECTOR CreateXMVector_XY(float x, float y) {
	using namespace DirectX;
	return XMVectorSet(x, y, 0, 0); 
}

DirectX::XMVECTOR CreateXMVector_XZ(float x, float z) {
	using namespace DirectX;
	return XMVectorSet(x, 0, z, 0);
}

DirectX::XMVECTOR CreateXMVector_YZ(float y, float z) {
	using namespace DirectX;
	return XMVectorSet(0, y, z, 0);
}

DirectX::XMVECTOR CreateXMVector_X(float x) {
	using namespace DirectX;
	return XMVectorSet(x, 0, 0, 0);
}

DirectX::XMVECTOR CreateXMVector_Y(float y) {
	using namespace DirectX;
	return XMVectorSet(0, y, 0, 0);
}

DirectX::XMVECTOR CreateXMVector_Z(float z) {
	using namespace DirectX;
	return XMVectorSet(0, 0, z, 0);
}

#endif