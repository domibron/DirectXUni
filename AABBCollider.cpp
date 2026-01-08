#include "AABBCollider.h"

#include "Transform.h"

#include <cmath>

AABBCollider::AABBCollider(Transform* transform, float xSize, float ySize, float zSize, float xPos, float yPos, float zPos)
	:boxTrans(transform)
{
	colliderData.xSize = xSize;
	colliderData.ySize = ySize;
	colliderData.zSize = zSize;
	colliderData.xPos = xPos;
	colliderData.yPos = yPos;
	colliderData.zPos = zPos;
}

AABBCollider::AABBCollider(Transform* transform, AABBData data)
	:boxTrans(transform)
{
	// needs testing but the reason why im not doing colliderData = data is im unsure if it copies the address rather than fill it with the data.
	colliderData.xSize = data.xSize;
	colliderData.ySize = data.ySize;
	colliderData.zSize = data.zSize;
	colliderData.xPos = data.xPos;
	colliderData.yPos = data.yPos;
	colliderData.zPos = data.zPos;
}

bool AABBCollider::CheckForCollision( AABBData secondObject, DirectX::XMVECTOR secondObjectPos)
{
	DirectX::XMVECTOR firstObjectPosition = boxTrans->position;

	// a lost of assigning things, we ball with memory.
	// Get the xyz pos of the first object's transform.
	float fXPos = DirectX::XMVectorGetX(firstObjectPosition);
	float fYPos = DirectX::XMVectorGetY(firstObjectPosition);
	float fZPos = DirectX::XMVectorGetZ(firstObjectPosition);

	// Get the xyz pos of the second object's transform.
	float sXPos = DirectX::XMVectorGetX(secondObjectPos);
	float sYPos = DirectX::XMVectorGetY(secondObjectPos);
	float sZPos = DirectX::XMVectorGetZ(secondObjectPos);

	// Get the collider's world space pos for the first object.
	float offsetInWorldFX = colliderData.xPos + fXPos;
	float offsetInWorldFY = colliderData.yPos + fYPos;
	float offsetInWorldFZ = colliderData.zPos + fZPos;

	// Get the collider's world space pos for the second object.
	float offsetInWorldSX = secondObject.xPos + sXPos;
	float offsetInWorldSY = secondObject.yPos + sYPos;
	float offsetInWorldSZ = secondObject.zPos + sZPos;

	// get the bounds of the first object.
	float fxMin = offsetInWorldFX - (colliderData.xSize / 2.0f);
	float fxMax = offsetInWorldFX + (colliderData.xSize / 2.0f);
	float fyMin = offsetInWorldFY - (colliderData.ySize / 2.0f);
	float fyMax = offsetInWorldFY + (colliderData.ySize / 2.0f);
	float fzMin = offsetInWorldFZ - (colliderData.zSize / 2.0f);
	float fzMax = offsetInWorldFZ + (colliderData.zSize / 2.0f);

	// get the bounds of the second object.
	float sxMin = offsetInWorldSX - (secondObject.xSize / 2.0f);
	float sxMax = offsetInWorldSX + (secondObject.xSize / 2.0f);
	float syMin = offsetInWorldSY - (secondObject.ySize / 2.0f);
	float syMax = offsetInWorldSY + (secondObject.ySize / 2.0f);
	float szMin = offsetInWorldSZ - (secondObject.zSize / 2.0f);
	float szMax = offsetInWorldSZ + (secondObject.zSize / 2.0f);

	//bool xCollide = false, yCollide = false, zCollide = false;

	// possible failure of collison if fist min is less than second min and first max is greater than second max.
	if (fxMax > sxMin && fxMin < sxMax && 
		fyMax > syMin && fyMin < syMax &&
		fzMax > szMin && fzMin < szMax){
		return true;// a collision has taken place.
	}
	else return false;

	//return true; // a collision has taken place.
}

DirectX::XMVECTOR AABBCollider::GetOverlapAmount( AABBData secondObject, DirectX::XMVECTOR secondObjectPos)
{
	DirectX::XMVECTOR firstObjectPosition = boxTrans->position;

	
	// a lost of assigning things, we ball with memory.
	// Get the xyz pos of the first object's transform.
	float fXPos = DirectX::XMVectorGetX(firstObjectPosition);
	float fYPos = DirectX::XMVectorGetY(firstObjectPosition);
	float fZPos = DirectX::XMVectorGetZ(firstObjectPosition);

	// Get the xyz pos of the second object's transform.
	float sXPos = DirectX::XMVectorGetX(secondObjectPos);
	float sYPos = DirectX::XMVectorGetY(secondObjectPos);
	float sZPos = DirectX::XMVectorGetZ(secondObjectPos);

	// Get the collider's world space pos for the first object.
	float offsetInWorldFX = colliderData.xPos + fXPos;
	float offsetInWorldFY = colliderData.yPos + fYPos;
	float offsetInWorldFZ = colliderData.zPos + fZPos;

	// Get the collider's world space pos for the second object.
	float offsetInWorldSX = secondObject.xPos + sXPos;
	float offsetInWorldSY = secondObject.yPos + sYPos;
	float offsetInWorldSZ = secondObject.zPos + sZPos;

	// get the bounds of the first object.
	float fxMin = offsetInWorldFX - (colliderData.xSize / 2.0f);
	float fxMax = offsetInWorldFX + (colliderData.xSize / 2.0f);
	float fyMin = offsetInWorldFY - (colliderData.ySize / 2.0f);
	float fyMax = offsetInWorldFY + (colliderData.ySize / 2.0f);
	float fzMin = offsetInWorldFZ - (colliderData.zSize / 2.0f);
	float fzMax = offsetInWorldFZ + (colliderData.zSize / 2.0f);

	// get the bounds of the second object.
	float sxMin = offsetInWorldSX - (secondObject.xSize / 2.0f);
	float sxMax = offsetInWorldSX + (secondObject.xSize / 2.0f);
	float syMin = offsetInWorldSY - (secondObject.ySize / 2.0f);
	float syMax = offsetInWorldSY + (secondObject.ySize / 2.0f);
	float szMin = offsetInWorldSZ - (secondObject.zSize / 2.0f);
	float szMax = offsetInWorldSZ + (secondObject.zSize / 2.0f);

	float x, y, z;

	if (fxMax > sxMin && fxMin < sxMax) {
		//x = sxMin - fxMax;
		float aOverlap = abs(fxMax - sxMin);
		float bOverlap = abs(sxMax - fxMin);


		if (aOverlap < bOverlap)
			x = -aOverlap;
		else
			x = bOverlap;
	}
	else {
		x = 0;
	}


	if (fyMax > syMin && fyMin < syMax) {
		//y = syMin - fyMax;
		float aOverlap = abs(fyMax - syMin);
		float bOverlap = abs(syMax - fyMin);
		
		if (aOverlap < bOverlap)
			y = -aOverlap;
		else
			y = bOverlap;
	}
	else {
		y = 0;
	}


	if (fzMax > szMin && fzMin < szMax) {
		//z = szMin - fzMax;
		float aOverlap = abs(fzMax - szMin);
		float bOverlap = abs(szMax - fzMin);
		
		if (aOverlap < bOverlap)
			z = -aOverlap;
		else
			z = bOverlap;
	}
	else {
		z = 0;
	}


	return DirectX::XMVectorSet(x, y, z, 1);
}

DirectX::XMVECTOR AABBCollider::WhichSideIsFistCollision(AABBData secondObject, DirectX::XMVECTOR secondObjectPos)
{
	DirectX::XMVECTOR firstObjectPosition = boxTrans->position;


	// a lost of assigning things, we ball with memory.
	// Get the xyz pos of the first object's transform.
	float fXPos = DirectX::XMVectorGetX(firstObjectPosition);
	float fYPos = DirectX::XMVectorGetY(firstObjectPosition);
	float fZPos = DirectX::XMVectorGetZ(firstObjectPosition);

	// Get the xyz pos of the second object's transform.
	float sXPos = DirectX::XMVectorGetX(secondObjectPos);
	float sYPos = DirectX::XMVectorGetY(secondObjectPos);
	float sZPos = DirectX::XMVectorGetZ(secondObjectPos);

	// Get the collider's world space pos for the first object.
	float offsetInWorldFX = colliderData.xPos + fXPos;
	float offsetInWorldFY = colliderData.yPos + fYPos;
	float offsetInWorldFZ = colliderData.zPos + fZPos;

	// Get the collider's world space pos for the second object.
	float offsetInWorldSX = secondObject.xPos + sXPos;
	float offsetInWorldSY = secondObject.yPos + sYPos;
	float offsetInWorldSZ = secondObject.zPos + sZPos;

	//// get the bounds of the first object.
	//float fxMin = offsetInWorldFX - (colliderData.xSize / 2.0f);
	//float fxMax = offsetInWorldFX + (colliderData.xSize / 2.0f);
	//float fyMin = offsetInWorldFY - (colliderData.ySize / 2.0f);
	//float fyMax = offsetInWorldFY + (colliderData.ySize / 2.0f);
	//float fzMin = offsetInWorldFZ - (colliderData.zSize / 2.0f);
	//float fzMax = offsetInWorldFZ + (colliderData.zSize / 2.0f);

	//// get the bounds of the second object.
	//float sxMin = offsetInWorldSX - (secondObject.xSize / 2.0f);
	//float sxMax = offsetInWorldSX + (secondObject.xSize / 2.0f);
	//float syMin = offsetInWorldSY - (secondObject.ySize / 2.0f);
	//float syMax = offsetInWorldSY + (secondObject.ySize / 2.0f);
	//float szMin = offsetInWorldSZ - (secondObject.zSize / 2.0f);
	//float szMax = offsetInWorldSZ + (secondObject.zSize / 2.0f);

	//float x = 0 , y = 0, z = 0;

	DirectX::XMVECTOR CollisionDir = DirectX::XMVectorSet(offsetInWorldSX - offsetInWorldFX, offsetInWorldSY - offsetInWorldFY, offsetInWorldSZ - offsetInWorldFZ, 0);
	//CollisionDir = DirectX::XMVector3Normalize(CollisionDir);

	bool posX = DirectX::XMVectorGetX(CollisionDir) >= 0;
	float xDotRes = (posX ? DotProduct(CollisionDir, DirectX::XMVectorSet(1.0f, 0, 0, 0)) : DotProduct(CollisionDir, DirectX::XMVectorSet(1.0f,0, 0, 0)));

	bool posY = DirectX::XMVectorGetY(CollisionDir) >= 0;
	float yDotRes = (posY ? DotProduct(CollisionDir, DirectX::XMVectorSet(0, 1.0f, 0, 0)) : DotProduct(CollisionDir, DirectX::XMVectorSet(0, -1.0f, 0, 0)));

	bool posZ = DirectX::XMVectorGetZ(CollisionDir) >= 0;
	float zDotRes = (posZ ? DotProduct(CollisionDir, DirectX::XMVectorSet(0, 0, 1.0f, 0)) : DotProduct(CollisionDir, DirectX::XMVectorSet(0, 0, -1.0f, 0)));


	if (xDotRes > yDotRes && xDotRes > zDotRes) {
		return DirectX::XMVectorSet((posX ? 1.0f: -1.0f), 0, 0, 0);
	}
	else if (yDotRes > zDotRes) {
		return DirectX::XMVectorSet(0, (posY ? 1.0f : -1.0f), 0, 0);
	}
	else {
		return DirectX::XMVectorSet(0, 0, (posZ ? 1.0f : -1.0f), 0);
	}
}

float AABBCollider::DotProduct(DirectX::XMVECTOR a, DirectX::XMVECTOR b) {
	return (DirectX::XMVectorGetX(a) * DirectX::XMVectorGetX(b)) + (DirectX::XMVectorGetY(a) * DirectX::XMVectorGetY(b)) + (DirectX::XMVectorGetZ(a) * DirectX::XMVectorGetZ(b));
}
