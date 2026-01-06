#pragma once

#include "GameObject.h"

# include "AABBCollider.h"

class BlockMesh;

class BlockObject
	: public GameObject, public AABBCollider
{
private:
	bool backFace = true;
	bool topFace = true;
	bool frontFace = true;
	bool bottomFace = true;
	bool rightFace = true;
	bool leftFace = true;

public:

	BlockMesh* blockMesh;

	BlockObject(std::string objectName, BlockMesh* objectMesh, Material* material);

	void SetBlockRenderFaces(bool back = true, bool top = true, bool front = true, bool bottom = true, bool right = true, bool left = true);

	virtual void RenderObject() override;
	//BlockObject(BlockObject&&) = default;
	//BlockObject& operator=(BlockObject&&) = default;

	//BlockObject(const BlockObject&) = delete;
	//BlockObject& operator=(const BlockObject&) = delete;
};

