#include "AABBCollider.h"


AABBCollider::AABBCollider(float xSize, float ySize, float zSize, float xPos, float yPos, float zPos)
{
	colliderData.xSize = xSize;
	colliderData.ySize = ySize;
	colliderData.zSize = zSize;
	colliderData.xPos = xPos;
	colliderData.yPos = yPos;
	colliderData.zPos = zPos;
}

AABBCollider::AABBCollider(AABBData data)
{
	// needs testing but the reason why im not doing colliderData = data is im unsure if it copies the address rather than fill it with the data.
	colliderData.xSize = data.xSize;
	colliderData.ySize = data.ySize;
	colliderData.zSize = data.zSize;
	colliderData.xPos = data.xPos;
	colliderData.yPos = data.yPos;
	colliderData.zPos = data.zPos;
}

bool AABBCollider::CheckForCollision(AABBData firstObject, DirectX::XMVECTOR firstObjectPosition, AABBData secondObject, DirectX::XMVECTOR secondObjectPos)
{
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
	float offsetInWorldFX = firstObject.xPos + fXPos;
	float offsetInWorldFY = firstObject.yPos + fYPos;
	float offsetInWorldFZ = firstObject.zPos + fZPos;

	// Get the collider's world space pos for the second object.
	float offsetInWorldSX = secondObject.xPos + sXPos;
	float offsetInWorldSY = secondObject.yPos + sYPos;
	float offsetInWorldSZ = secondObject.zPos + sZPos;

	// get the bounds of the first object.
	float fxMin = offsetInWorldFX - (firstObject.xSize / 2.0f);
	float fxMax = offsetInWorldFX + (firstObject.xSize / 2.0f);
	float fyMin = offsetInWorldFY - (firstObject.ySize / 2.0f);
	float fyMax = offsetInWorldFY + (firstObject.ySize / 2.0f);
	float fzMin = offsetInWorldFZ - (firstObject.zSize / 2.0f);
	float fzMax = offsetInWorldFZ + (firstObject.zSize / 2.0f);

	// get the bounds of the second object.
	float sxMin = offsetInWorldSX - (secondObject.xSize / 2.0f);
	float sxMax = offsetInWorldSX + (secondObject.xSize / 2.0f);
	float syMin = offsetInWorldSY - (secondObject.ySize / 2.0f);
	float syMax = offsetInWorldSY + (secondObject.ySize / 2.0f);
	float szMin = offsetInWorldSZ - (secondObject.zSize / 2.0f);
	float szMax = offsetInWorldSZ + (secondObject.zSize / 2.0f);

	bool xCollide = false, yCollide = false, zCollide = false;

	if (fxMax > sxMin || fxMin < sxMax) {
		xCollide = true;
	}
	else return false;

	if (fyMax > syMin || fyMin < syMax) {
		yCollide = true;
	}
	else return false;

	if (fzMax > szMin || fzMin < szMax) {
		zCollide = true;
	}
	else return false;

	return true; // a collision has taken place.
}

DirectX::XMVECTOR AABBCollider::GetOverlapAmount(AABBData firstObject, DirectX::XMVECTOR firstObjectPosition, AABBData secondObject, DirectX::XMVECTOR secondObjectPos)
{
	// we presume we are colliding, its is on the hands of the other systems to make sure we collided.

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
	float offsetInWorldFX = firstObject.xPos + fXPos;
	float offsetInWorldFY = firstObject.yPos + fYPos;
	float offsetInWorldFZ = firstObject.zPos + fZPos;

	// Get the collider's world space pos for the second object.
	float offsetInWorldSX = secondObject.xPos + sXPos;
	float offsetInWorldSY = secondObject.yPos + sYPos;
	float offsetInWorldSZ = secondObject.zPos + sZPos;

	// get the bounds of the first object.
	float fxMin = offsetInWorldFX - (firstObject.xSize / 2.0f);
	float fxMax = offsetInWorldFX + (firstObject.xSize / 2.0f);
	float fyMin = offsetInWorldFY - (firstObject.ySize / 2.0f);
	float fyMax = offsetInWorldFY + (firstObject.ySize / 2.0f);
	float fzMin = offsetInWorldFZ - (firstObject.zSize / 2.0f);
	float fzMax = offsetInWorldFZ + (firstObject.zSize / 2.0f);

	// get the bounds of the second object.
	float sxMin = offsetInWorldSX - (secondObject.xSize / 2.0f);
	float sxMax = offsetInWorldSX + (secondObject.xSize / 2.0f);
	float syMin = offsetInWorldSY - (secondObject.ySize / 2.0f);
	float syMax = offsetInWorldSY + (secondObject.ySize / 2.0f);
	float szMin = offsetInWorldSZ - (secondObject.zSize / 2.0f);
	float szMax = offsetInWorldSZ + (secondObject.zSize / 2.0f);

	float x, y, z;

	if (fxMax > sxMin) {
		x = fxMax - sxMin;
	}
	else if (fxMin < sxMax) {
		x = fxMin - sxMax;
	}
	else {
		x = 0;
	}

	if (fyMax > syMin) {
		y = fyMax - syMin;
	}
	else if (fyMin < syMax) {
		y = fyMin - syMax;
	}
	else {
		y = 0;
	}

	if (fzMax > szMin) {
		z = fzMax - szMin;
	}
	else if (fzMin < szMax) {
		z = fzMin - szMax;
	}
	else {
		z = 0;
	}

	return DirectX::XMVectorSet(x, y, z, 0);
}
