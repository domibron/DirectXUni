#include "ChunkData.h"
#include "GameObject.h"
#include "Material.h"
#include "Mesh.h"
#include "Renderer.h" // Oh god.
#include "memory.h"
#include "BlockObject.h"
#include "PhysicsHanderler.h"

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

	for (auto& pair : blocksInChunk) {
		renderer->RegisterGameObject(pair.second.get());
	}

	//for (int i = 0; i < blocksInChunk.size(); i++) {
	//	renderer->RegisterGameObject(blocksInChunk[i].get());
	//}

}

void ChunkData::UnloadChunk()
{
	for (auto& pair : blocksInChunk) {
		renderer->RemoveGameObject(pair.second.get());
	}

	//for (int i = 0; i < blocksInChunk.size(); i++) {
	//	renderer->RemoveGameObject(blocksInChunk[i].get());
	//}
}

bool ChunkData::BlockOcupyingChunkPos(DirectX::XMVECTOR position)
{
	using namespace DirectX;

	if (XMVectorGetX(position) < 0 || XMVectorGetX(position) >= WIDTH) {
		return false; // we need to check adjacent chunks.
	}
	if (XMVectorGetZ(position) < 0 || XMVectorGetZ(position) >= DEPTH) {
		return false; // we need to check adjacent chunks.
	}

	if (XMVectorGetY(position) < 0 || XMVectorGetY(position) >= height) {
		return false; // we need to check adjacent chunks.
	}

	auto key = blocksInChunk.find(BlockPosition{ XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position) });
	if (key == blocksInChunk.end())
	{
		return false;
	}

	return true; // we dont need to iterate, just know the key exsists.
}

ChunkData::ChunkData(Renderer* renderer, PhysicsHanderler* physicsHanderler , DirectX::XMVECTOR chunkPosition, BlockMesh* blockMesh, Material* material)
	: renderer(renderer), pHanderler(physicsHanderler)
{
	// we shall bullshit the gerator for now
	chunkTransform.position = chunkPosition;

	using namespace DirectX;

	XMVECTOR targetPos = XMVectorSet(0,0,0,1);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < WIDTH; x++) {
			for (int z = 0; z < DEPTH; z++) {

				
				BlockObject newBlock{ "Block", blockMesh, material };
				targetPos = XMVectorSet(x, y, z, 1) + chunkTransform.position;

				BlockPosition posInChunk{ x, y, z };
				
				newBlock.transform.position = targetPos;
				//GameObject temp("Block", blockMesh, material);
				blocksInChunk.try_emplace(posInChunk, std::make_unique<BlockObject>(newBlock));
				//blocksInChunk.push_back(std::make_unique<GameObject>(newBlock));

			}
		}
	}


	for (auto& pair : blocksInChunk) {

		// add teh blocks to physisc.
		pHanderler->RegisterStaticBody(pair.second.get());

		// toggle which faces are visible.

		BlockPosition pos = pair.first;
		XMVECTOR vectorPos = XMVectorSet(pos.x, pos.y, pos.z, 1);

		BlockObject* blockPtr = pair.second.get();

		bool showBackFace, showTopFace, showFrontFace, showBottomFace, showRightFace, showLeftFace;

		XMVECTOR res = vectorPos + XMVectorSet(0, 0, 1, 0);


		showBackFace	=	!BlockOcupyingChunkPos(vectorPos	+ XMVectorSet( 0,    0,    1.0f, 0));
		showFrontFace	=	!BlockOcupyingChunkPos(vectorPos	+ XMVectorSet( 0,    0,   -1.0f, 0));
		showBottomFace	=	!BlockOcupyingChunkPos(vectorPos	+ XMVectorSet( 0,   -1.0f, 0,    0));
		showTopFace		=	!BlockOcupyingChunkPos(vectorPos	+ XMVectorSet( 0,    1.0f, 0,    0));
		showLeftFace	=	!BlockOcupyingChunkPos(vectorPos	+ XMVectorSet(-1.0f, 0,    0,    0));
		showRightFace	=	!BlockOcupyingChunkPos(vectorPos	+ XMVectorSet( 1.0f, 0,    0,    0));

		blockPtr->SetBlockRenderFaces(showBackFace, showTopFace, showFrontFace, showBottomFace, showRightFace, showLeftFace);
	}
}
