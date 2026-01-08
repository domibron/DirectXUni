#include "PhysicsHanderler.h"

//#include "RigidBody.h"

#include "TimeKeeping.h"

#include "MovingEntity.h"
#include "BlockObject.h"

#include "CollisionDetection.h"
#include "Ray.h"

#include "Transform.h"

#include <iostream>
#include <vector>

#include <cmath>

float Dist(DirectX::XMVECTOR a, DirectX::XMVECTOR b);

PhysicsHanderler::PhysicsHanderler(TimeKeeping* tKeeping, Transform* cameraTransform)
	: timeKeeping(tKeeping), camTransform(cameraTransform)
{

}

void PhysicsHanderler::RegisterStaticBody(BlockObject* staticObject)
{
	blocks.push_back(staticObject);
}

void PhysicsHanderler::RegisterRigidBody(MovingEntity* rigidBody)
{
	movingEntities.push_back(rigidBody);
}

void PhysicsHanderler::RemoveStaticBody(BlockObject* staticObject)
{
	auto foundEntity = std::find(blocks.begin(), blocks.end(), staticObject);
	if (foundEntity != blocks.end()) {
		blocks.erase(foundEntity);
	}
}

void PhysicsHanderler::TickPhysics()
{
	float deltaForThisFrame = timeKeeping->GetDeltaTime();

	for (auto movingEntity : movingEntities) {
		DirectX::XMVECTOR beforePos = movingEntity->transform.position;

		movingEntity->TickThisRigidBodyPhysics(deltaForThisFrame); // move the object.

		// check for collision with all blocks. Can be optimised to use only blocks within radius.
		for (auto staticObject : blocks){
			if (movingEntity->CheckForCollision(staticObject->GetBounds(), staticObject->transform.position)) {
				// collision detected!
				//std::cout << "Collision" << std::endl;

				DirectX::XMVECTOR overlapAmount = movingEntity->GetOverlapAmount(staticObject->GetBounds(), staticObject->transform.position);

				// do something to the velocity.
				DirectX::XMVECTOR collisionSide = movingEntity->WhichSideIsFistCollision(staticObject->GetBounds(), staticObject->transform.position);

				DirectX::XMVECTOR vel = movingEntity->GetVelocity();

				DirectX::XMVECTOR moveAmount{ 0,0,0,0 };
				// We know one face is the closest collision, there is a issue if the object is moving fast enough it will skip physics.
				// Could add some bounce here.
				if (DirectX::XMVectorGetX(collisionSide) != 0) {
					moveAmount = DirectX::XMVectorSet(-DirectX::XMVectorGetX(overlapAmount), 0, 0, 0);

					if (DirectX::XMVectorGetX(vel) > 0 && DirectX::XMVectorGetX(collisionSide) > 0)
						movingEntity->SetVelocityX(0);
					else if (DirectX::XMVectorGetX(vel) < 0 && DirectX::XMVectorGetX(collisionSide) < 0)
						movingEntity->SetVelocityX(0);
				}
				else if (DirectX::XMVectorGetY(collisionSide) != 0) {
					moveAmount = DirectX::XMVectorSet(0, -DirectX::XMVectorGetY(overlapAmount), 0, 0);
					
					if (DirectX::XMVectorGetY(vel) > 0 && DirectX::XMVectorGetY(collisionSide) > 0)
						movingEntity->SetVelocityY(0);
					else if (DirectX::XMVectorGetY(vel) < 0 && DirectX::XMVectorGetY(collisionSide) < 0)
						movingEntity->SetVelocityY(0);
				}
				else if (DirectX::XMVectorGetZ(collisionSide) != 0) {
					moveAmount = DirectX::XMVectorSet(0, 0, -DirectX::XMVectorGetZ(overlapAmount), 0);
					
					if (DirectX::XMVectorGetZ(vel) > 0 && DirectX::XMVectorGetZ(collisionSide) > 0)
						movingEntity->SetVelocityZ(0);
					else if (DirectX::XMVectorGetZ(vel) < 0 && DirectX::XMVectorGetZ(collisionSide) < 0)
						movingEntity->SetVelocityZ(0);
				}

				// Move the entity to the closet point to the collision.
				movingEntity->transform.position = DirectX::XMVectorAdd(beforePos, moveAmount);
			}
		}
	}


	DirectX::XMVECTOR camPos = camTransform->position;

	Ray ray{ camPos, camTransform->GetForward() };
	std::vector<RayCollision<BlockObject>> collisionRes;
	CollisionDetection cd;

	std::vector<BlockObject*> hitObjects;

	for (auto staticObject : blocks) {
		RayCollision<BlockObject> colision;

		if (cd.RayAABBIntersection(ray, staticObject, colision))
		{
			//std::cout << "HIT" << std::endl;
			//std::cout << DirectX::XMVectorGetX(camTransform->GetForward()) << std::endl;
			hitObjects.push_back(staticObject);
			collisionRes.push_back(colision);
		}
	}


	if (hitObjects.size() > 0) {
		DirectX::XMVECTOR closestPos;
		float lastDist = -1;
		int index = 0;

		for (int i = 0; i < hitObjects.size(); i++) {
			if (i == 0) {
				closestPos = hitObjects[i]->transform.position;
				lastDist = Dist(camPos, hitObjects[i]->transform.position);
				index = i;
			}
			else if (Dist(camPos, hitObjects[i]->transform.position) < lastDist) {
				closestPos = hitObjects[i]->transform.position;
				lastDist = Dist(camPos, hitObjects[i]->transform.position);
				index = i;				
			}
		}

		slectedBlock = hitObjects[index];
		hitPoint = collisionRes[index].collidedAt;
	}
	else {
		slectedBlock = nullptr;
	}
}

float Dist(DirectX::XMVECTOR a, DirectX::XMVECTOR b) {
	using namespace DirectX;

	float x = XMVectorGetX(a) - XMVectorGetX(b);
	float y = XMVectorGetY(a) - XMVectorGetY(b);
	float z = XMVectorGetZ(a) - XMVectorGetZ(b);

	return sqrt((x * x) + (y * y) + (z * z));
}
