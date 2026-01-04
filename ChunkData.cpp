#include "ChunkData.h"
#include "GameObject.h"
#include "Material.h"
#include "Mesh.h"
#include "Renderer.h" // Oh god.
#include "memory.h"

//void ChunkData::AddBlockToChunk(GameObject* block)
//{
//	
//	blocksInChunk.push_back(block);
//}
//
//void ChunkData::RemoveBlockFromChunk(GameObject* block)
//{
//	auto foundEntity = std::find(blocksInChunk.begin(), blocksInChunk.end(), block);
//
//	if (foundEntity != blocksInChunk.end()) {
//		blocksInChunk.erase(foundEntity);
//	}
//}

void ChunkData::LoadChunk()
{
	//if (blocksInChunk.size() <= 0) return;

	for (int i = 0; i < blocksInChunk.size(); i++) {
		renderer->RegisterGameObject(blocksInChunk[i].get());
	}

}

void ChunkData::UnloadChunk()
{
	for (int i = 0; i < blocksInChunk.size(); i++) {
		renderer->RemoveGameObject(blocksInChunk[i].get());
	}
}

ChunkData::ChunkData(Renderer* renderer, DirectX::XMVECTOR chunkPosition, Mesh* blockMesh, Material* material)
	: renderer(renderer)
{
	// we shall bullshit the gerator for now
	chunkTransform.position = chunkPosition;

	using namespace DirectX;

	XMVECTOR targetPos = XMVectorSet(0,0,0,0);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int z = 0; z < depth; z++) {

				
				GameObject newBlock{ "Block", blockMesh, material };
				targetPos = XMVectorSet(x, y, z, 0) + chunkTransform.position;
				
				newBlock.transform.position = targetPos;
				//GameObject temp("Block", blockMesh, material);
				blocksInChunk.push_back(std::make_unique<GameObject>(newBlock));

			}
		}
	}
}
