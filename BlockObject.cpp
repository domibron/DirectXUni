#include "BlockObject.h"

#include "BlockMesh.h"

#include "Transform.h"

BlockObject::BlockObject(std::string objectName, BlockMesh* objectMesh, Material* material)
	: GameObject(objectName, (Mesh*)objectMesh, material), blockMesh(objectMesh), AABBCollider(&transform,1,1,1,0.5f,0.5f,0.0f) // urm bad shit, collider should not have TRANSFORM!!!!
{

}

void BlockObject::SetBlockRenderFaces(bool back, bool top, bool front, bool bottom, bool right, bool left)
{
	backFace = back;
	topFace = top;
	frontFace = front;
	bottomFace = bottom;
	rightFace = right;
	leftFace = left;
}

void BlockObject::RenderObject()
{
	blockMesh->SetVisibleFaces(backFace, topFace, frontFace, bottomFace, rightFace, leftFace);
	blockMesh->Render();
}
