#include "CollisionDetection.h"
#include "DirectXMath.h"


#include "AABBCollider.h"

#include "Ray.h"

bool CollisionDetection::RayAABBIntersection(Ray& r, DirectX::XMVECTOR& pos, AABBCollider& collider, RayCollision<BlockObject>& collision)
{
    using namespace DirectX;


    AABBData data = collider.GetBounds();

    XMVECTOR boxMin = pos + XMVectorSet(data.xPos, data.yPos, data.zPos, 0) - XMVectorSet(data.xSize / 2.0f, data.ySize / 2.0f, data.zSize / 2.0f, 0);
    XMVECTOR boxMax = pos + XMVectorSet(data.xPos, data.yPos, data.zPos, 0) + XMVectorSet(data.xSize / 2.0f, data.ySize / 2.0f, data.zSize / 2.0f, 0);

    XMVECTOR rayPos = r.GetPosition();
    XMVECTOR rayDir = r.GetDirection();


    XMVECTOR tVals = XMVectorSet(-1, -1, -1, 0);

    //x
    if (XMVectorGetX(rayDir) > 0) {
        XMVectorSetX(tVals, (XMVectorGetX(boxMin) - XMVectorGetX(rayPos)) / XMVectorGetX(rayDir));
    }
    else if (XMVectorGetX(rayDir) < 0) {
        XMVectorSetX(tVals, (XMVectorGetX(boxMax) - XMVectorGetX(rayPos)) / XMVectorGetX(rayDir));
    }

    //y
    if (XMVectorGetY(rayDir) > 0) {
        XMVectorSetY(tVals, (XMVectorGetY(boxMin) - XMVectorGetY(rayPos)) / XMVectorGetY(rayDir));
    }
    else if (XMVectorGetY(rayDir) < 0) {
        XMVectorSetY(tVals, (XMVectorGetY(boxMax) - XMVectorGetY(rayPos)) / XMVectorGetY(rayDir));
    }

    //z
    if (XMVectorGetZ(rayDir) > 0) {
        XMVectorSetZ(tVals, (XMVectorGetZ(boxMin) - XMVectorGetZ(rayPos)) / XMVectorGetZ(rayDir));
    }
    else if (XMVectorGetZ(rayDir) < 0) {
        XMVectorSetZ(tVals, (XMVectorGetZ(boxMax) - XMVectorGetZ(rayPos)) / XMVectorGetZ(rayDir));
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


    collision.collidedAt = intersection;
    collision.rayDistance = bestT;

    return true;
}
