#include "Camera.h"

using namespace DirectX;

Camera::Camera(Transform* transform)
    : camTransform(transform)
{
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{
    XMVECTOR eyePos = camTransform->position;
    XMVECTOR lookAt = camTransform->GetForward();
    XMVECTOR camUp = camTransform->GetUp();

    return XMMatrixLookToLH(eyePos, lookAt, camUp);
}

DirectX::XMMATRIX Camera::GetProjectionMatrix(int screenWidth, int screenHeight)
{
    return XMMatrixPerspectiveFovLH(
        XMConvertToRadians(fov),
        (float)screenWidth / (float)screenHeight,
        nearClippingPlane,
        farClippingPlane
    );
}
