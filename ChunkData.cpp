#include "ChunkData.h"
#include "GameObject.h"
#include "Material.h"
#include "Mesh.h"
#include "Renderer.h" // Oh god.
#include "memory.h"
#include "BlockObject.h"
#include "PhysicsHanderler.h"
#include <iostream>

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


void ChunkData::UpdateAllBlockFaces()
{
	using namespace DirectX;
	for (auto& pair : blocksInChunk) {

		
		// toggle which faces are visible.

		BlockPosition pos = pair.first;
		XMVECTOR vectorPos = XMVectorSet(pos.x, pos.y, pos.z, 1);

		BlockObject* blockPtr = pair.second.get();

		bool showBackFace, showTopFace, showFrontFace, showBottomFace, showRightFace, showLeftFace;

		XMVECTOR res = vectorPos + XMVectorSet(0, 0, 1, 0);


		showBackFace = !BlockOcupyingChunkPos(vectorPos + XMVectorSet(0, 0, 1.0f, 0));
		showFrontFace = !BlockOcupyingChunkPos(vectorPos + XMVectorSet(0, 0, -1.0f, 0));
		showBottomFace = !BlockOcupyingChunkPos(vectorPos + XMVectorSet(0, -1.0f, 0, 0));
		showTopFace = !BlockOcupyingChunkPos(vectorPos + XMVectorSet(0, 1.0f, 0, 0));
		showLeftFace = !BlockOcupyingChunkPos(vectorPos + XMVectorSet(-1.0f, 0, 0, 0));
		showRightFace = !BlockOcupyingChunkPos(vectorPos + XMVectorSet(1.0f, 0, 0, 0));

		blockPtr->SetBlockRenderFaces(showBackFace, showTopFace, showFrontFace, showBottomFace, showRightFace, showLeftFace);
	}
}

void ChunkData::LoadChunk()
{
	//if (blocksInChunk.size() <= 0) return;

	for (auto& pair : blocksInChunk) {
		renderer->RegisterGameObject(pair.second.get());
		// add teh blocks to physisc.
		pHanderler->RegisterStaticBody(pair.second.get());

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
}

void ChunkData::RemovedMarkedForDeleted()
{
	if (markedForRemoval.size() <= 0) return;


	for (auto marked : markedForRemoval) {
			
		renderer->RemoveGameObject(blocksInChunk[marked].get());
		pHanderler->RemoveStaticBody(blocksInChunk[marked].get());

		blocksInChunk.erase(marked);
		
	}

	markedForRemoval.clear();

	UpdateAllBlockFaces();
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

	auto key = blocksInChunk.find(BlockPosition{ static_cast<int>(XMVectorGetX(position)), static_cast<int>(XMVectorGetY(position)), static_cast<int>(XMVectorGetZ(position)) });
	if (key == blocksInChunk.end())
	{
		return false;
	}

	return true; // we dont need to iterate, just know the key exsists.
}

void ChunkData::AddBlockToLocation(DirectX::XMVECTOR positon)
{
	using namespace DirectX;

	BlockObject newBlock{ "Block", blockMesh, blockMat };
	XMVECTOR targetPos { static_cast<int>(XMVectorGetX(positon)), static_cast<int>(XMVectorGetY(positon)), static_cast<int>(XMVectorGetZ(positon)) }; // shoud set to whole numbers.

	XMVECTOR posBasedInChunk = positon - chunkTransform.position;

	BlockPosition posInChunk{ static_cast<int>(XMVectorGetX(posBasedInChunk)), static_cast<int>(XMVectorGetY(posBasedInChunk)), static_cast<int>(XMVectorGetZ(posBasedInChunk)) };


	if (posInChunk.y < 0 || posInChunk.y >= height) {
		//std::cout << "Height failed" << std::endl;
		return;
	}
	if (posInChunk.x < 0 || posInChunk.x >= WIDTH) {
		//std::cout << "X failed" << std::endl;
		return;
	}
	if (posInChunk.z < 0 || posInChunk.z >= DEPTH) {
		//std::cout << "Z failed" << std::endl;
		return;
	}

	if (BlockOcupyingChunkPos(posBasedInChunk)) {
		//std::cout << "Block occupy failed" << std::endl;
		return;
	}


	newBlock.transform.position = targetPos;
	//GameObject temp("Block", blockMesh, material);
	auto pair = blocksInChunk.try_emplace(posInChunk, std::make_unique<BlockObject>(newBlock));
	//blocksInChunk.push_back(std::make_unique<GameObject>(newBlock));

	auto blockDataPair = pair.first;

	renderer->RegisterGameObject(blockDataPair->second.get());
	pHanderler->RegisterStaticBody(blockDataPair->second.get());

	UpdateAllBlockFaces();

	//std::cout << "Placed block at X: " << posInChunk.x << " Y: " << posInChunk.y << " Z: " << posInChunk.z << " total: " << blocksInChunk.size();


}

void ChunkData::RemoveBlock(BlockObject* block)
{
	for (auto& pair : blocksInChunk) {
		if (pair.second.get() == block) {
			
			pair.second.get()->markedForDeletion = true;

			markedForRemoval.push_back(pair.first);
			break;
		}
	}

	//UpdateAllBlockFaces();
}

ChunkData::ChunkData(Renderer* renderer, PhysicsHanderler* physicsHanderler , DirectX::XMVECTOR chunkPosition, BlockMesh* blockMesh, Material* material)
	: renderer(renderer), pHanderler(physicsHanderler), blockMesh(blockMesh), blockMat(material)
{
	// we shall bullshit the gerator for now
	chunkTransform.position = chunkPosition;

	using namespace DirectX;

	XMVECTOR targetPos = XMVectorSet(0,0,0,1);

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < WIDTH; x++) {
			for (int z = 0; z < DEPTH; z++) {

				
				BlockObject newBlock{ "Block", blockMesh, blockMat };
				targetPos = XMVectorSet(x, y, z, 1) + chunkTransform.position;

				BlockPosition posInChunk{ x, y, z };
				
				newBlock.transform.position = targetPos;
				//GameObject temp("Block", blockMesh, material);
				blocksInChunk.try_emplace(posInChunk, std::make_unique<BlockObject>(newBlock));
				//blocksInChunk.push_back(std::make_unique<GameObject>(newBlock));

			}
		}
	}

	UpdateAllBlockFaces();
	
}
