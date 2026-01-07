#pragma once

#include <vector>
#include <memory>
#include "Transform.h" // pulls direct x math.
#include <unordered_map>


#define MAX_BLOCKS_IN_CHUNK 768
#define WIDTH 16
#define DEPTH 16

class Renderer;
//class GameObject;
class BlockObject;
class Mesh;
class Material;
class BlockMesh;
class PhysicsHanderler;


class ChunkData
{
private:
	struct BlockPosition {
		float x, y, z;
	};

	struct BlockPositionHash {
		size_t operator()(const BlockPosition& bp) const noexcept {
			size_t h1 = std::hash<float>{}(bp.x);
			size_t h2 = std::hash<float>{}(bp.y);
			size_t h3 = std::hash<float>{}(bp.z);
			return h1 ^ (h2 << 1) ^ (h3 << 2);
		}
	};

	struct BlockPositionEqual {
		bool operator()(const BlockPosition& a, const BlockPosition& b) const noexcept {
			return a.x == b.x && a.y == b.y && a.z == b.z;
		}
	};

	int height = 3;
	Renderer* renderer;
	PhysicsHanderler* pHanderler;

	Transform chunkTransform;

	std::unordered_map<BlockPosition, std::unique_ptr<BlockObject>, BlockPositionHash, BlockPositionEqual> blocksInChunk;

public:
	//void AddBlockToChunk(BlockObject* block);
	//void RemoveBlockFromChunk(BlockObject* block);
	void LoadChunk();
	void UnloadChunk();

	// position based from the chunk.
	bool BlockOcupyingChunkPos(DirectX::XMVECTOR position);

	//std::vector<std::unique_ptr<GameObject>> blocksInChunk;
	ChunkData(Renderer* renderer, PhysicsHanderler* physicsHanderler, DirectX::XMVECTOR chunkPosition, BlockMesh* blockMesh, Material* mat);
};

