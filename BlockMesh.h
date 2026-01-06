#pragma once

#include "Mesh.h"

class BlockMesh : 
	public Mesh
{
private:
	bool backFace = true;
	bool topFace = true;
	bool frontFace = true;
	bool bottomFace = true;
	bool rightFace = true;
	bool leftFace = true;

public:
	BlockMesh(Renderer& renderer);

	virtual void Render() override;

	// back, top, front, bottom, right, left.
	void SetVisibleFaces(bool back = true, bool top = true, bool front = true, bool bottom = true, bool right = true, bool left = true);
};

