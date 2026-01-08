#include "CollisionDetection.h"
#include "DirectXMath.h"


#include "AABBCollider.h"

#include "BlockObject.h"

#include "Ray.h"

bool CollisionDetection::RayAABBIntersection(Ray& r, BlockObject* collider, RayCollision<BlockObject>& collision)
{
    using namespace DirectX;


    AABBData data = collider->GetBounds();
    XMVECTOR boxPos = collider->transform.position;

    XMVECTOR boxMin = (boxPos + XMVectorSet(data.xPos, data.yPos, data.zPos, 0)) - XMVectorSet(data.xSize / 2.0f, data.ySize / 2.0f, data.zSize / 2.0f, 1);
    XMVECTOR boxMax = (boxPos + XMVectorSet(data.xPos, data.yPos, data.zPos, 0)) + XMVectorSet(data.xSize / 2.0f, data.ySize / 2.0f, data.zSize / 2.0f, 1);

    XMVECTOR rayPos = r.GetPosition();
    XMVECTOR rayDir = r.GetDirection();


    XMVECTOR tVals{-1.0f,-1.0f,-1.0f, 1.0f};

    //x
    if (XMVectorGetX(rayDir) > 0) {
        tVals = XMVectorSetX(tVals, (XMVectorGetX(boxMin) - XMVectorGetX(rayPos)) / XMVectorGetX(rayDir));
    }
    else if (XMVectorGetX(rayDir) < 0) {
        tVals = XMVectorSetX(tVals, (XMVectorGetX(boxMax) - XMVectorGetX(rayPos)) / XMVectorGetX(rayDir));
    }

    //y
    if (XMVectorGetY(rayDir) > 0) {
        tVals = XMVectorSetY(tVals, (XMVectorGetY(boxMin) - XMVectorGetY(rayPos)) / XMVectorGetY(rayDir));
    }
    else if (XMVectorGetY(rayDir) < 0) {
        tVals = XMVectorSetY(tVals, (XMVectorGetY(boxMax) - XMVectorGetY(rayPos)) / XMVectorGetY(rayDir));
    }

    //z
    if (XMVectorGetZ(rayDir) > 0) {
        tVals = XMVectorSetZ(tVals, (XMVectorGetZ(boxMin) - XMVectorGetZ(rayPos)) / XMVectorGetZ(rayDir));
    }
    else if (XMVectorGetZ(rayDir) < 0) {
        tVals = XMVectorSetZ(tVals, (XMVectorGetZ(boxMax) - XMVectorGetZ(rayPos)) / XMVectorGetZ(rayDir));
    }

    float bestT = XMVectorGetX(tVals);

    if (bestT < XMVectorGetY(tVals)) {
        bestT = XMVectorGetY(tVals);
    }

    if (bestT < XMVectorGetZ(tVals)) {
        bestT = XMVectorGetZ(tVals);
    }

    if (bestT < 0.0f) {
        return false; // no back facing rays
    }

    // TODO: Make sure the w being 0 does not cause issues.
    XMVECTOR intersection = rayPos + (rayDir * bestT);
    const float epsilon = 0.0001f; // leeway for calcs.
    if (XMVectorGetX(intersection) + epsilon < XMVectorGetX(boxMin) || XMVectorGetX(intersection) - epsilon > XMVectorGetX(boxMax)) {
        return false; // best intersection doesnt hit the box.
    }

    if (XMVectorGetY(intersection) + epsilon < XMVectorGetY(boxMin) || XMVectorGetY(intersection) - epsilon > XMVectorGetY(boxMax)) {
        return false; // best intersection doesnt hit the box.
    }

    if (XMVectorGetZ(intersection) + epsilon < XMVectorGetZ(boxMin) || XMVectorGetZ(intersection) - epsilon > XMVectorGetZ(boxMax)) {
        return false; // best intersection doesnt hit the box.
    }

    collision.node = collider;
    collision.collidedAt = intersection;
    collision.rayDistance = bestT;

    DirectX::XMVECTOR interDir = intersection - (boxPos + XMVectorSet(data.xPos, data.yPos, data.zPos, 0));

    bool posX = DirectX::XMVectorGetX(interDir) >= 0;
    float xDotRes = (posX ? DotProduct(interDir, DirectX::XMVectorSet(1.0f, 0, 0, 1)) : DotProduct(interDir, DirectX::XMVectorSet(1.0f, 0, 0, 1)));

    bool posY = DirectX::XMVectorGetY(interDir) >= 0;
    float yDotRes = (posY ? DotProduct(interDir, DirectX::XMVectorSet(0, 1.0f, 0, 1)) : DotProduct(interDir, DirectX::XMVectorSet(0, -1.0f, 0, 1)));

    bool posZ = DirectX::XMVectorGetZ(interDir) >= 0;
    float zDotRes = (posZ ? DotProduct(interDir, DirectX::XMVectorSet(0, 0, 1.0f, 1)) : DotProduct(interDir, DirectX::XMVectorSet(0, 0, -1.0f, 1)));


    if (xDotRes > yDotRes && xDotRes > zDotRes) {
       collision.normal = DirectX::XMVectorSet((posX ? 1.0f : -1.0f), 0, 0, 1);
    }
    else if (yDotRes > zDotRes) {
        collision.normal = DirectX::XMVectorSet(0, (posY ? 1.0f : -1.0f), 0, 1);
    }
    else {
        collision.normal = DirectX::XMVectorSet(0, 0, (posZ ? 1.0f : -1.0f), 1);
    }

    return true;
}

float CollisionDetection::DotProduct(DirectX::XMVECTOR a, DirectX::XMVECTOR b) {
    return (DirectX::XMVectorGetX(a) * DirectX::XMVectorGetX(b)) + (DirectX::XMVectorGetY(a) * DirectX::XMVectorGetY(b)) + (DirectX::XMVectorGetZ(a) * DirectX::XMVectorGetZ(b));
}
