#include "Camera.h"

DirectX::XMMATRIX Camera::GetViewMatrix()
{
    XMVECTOR eyePos = transform.position;
    XMVECTOR lookAt = transform.GetForward();
    XMVECTOR camUp = transform.GetUp();

    return XMMatrixLookAtLH(eyePos, lookAt, camUp);
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
