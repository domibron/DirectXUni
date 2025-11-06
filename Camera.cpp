#include "Camera.h"

using namespace DirectX;

DirectX::XMMATRIX Camera::GetViewMatrix()
{
    XMVECTOR eyePos = transform.position;
    XMVECTOR lookAt = transform.GetForward();
    XMVECTOR camUp = transform.GetUp();

    return XMMatrixLookToLH(eyePos, lookAt, camUp);
}

DirectX::XMMATRIX Camera::GetProjectionMatrix(int screenWidth, int screenHeight)
{
    return XMMatrixPerspectiveFovLH(
        XMConvertToRadians(fov),
        screenWidth / (float)screenHeight,
        nearClippingPlane,
        farClippingPlane
    );
}
