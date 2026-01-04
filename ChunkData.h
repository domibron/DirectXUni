#pragma once

#include <vector>
#include <memory>
#include "Transform.h" // pulls direct x math.


#define MAX_BLOCKS_IN_CHUNK 768

class Renderer;
class GameObject;
class Mesh;
class Material;

class ChunkData
{
private:
	int height = 3;
	int width = 16;
	int depth = 16;
	Renderer* renderer;

	Transform chunkTransform;

public:
	void AddBlockToChunk(GameObject* block);
	void RemoveBlockFromChunk(GameObject* block);
	void LoadChunk();
	void UnloadChunk();

	std::vector<std::unique_ptr<GameObject>> blocksInChunk;
	ChunkData(Renderer* renderer, DirectX::XMVECTOR chunkPosition, Mesh* blockMesh, Material* mat);
};

