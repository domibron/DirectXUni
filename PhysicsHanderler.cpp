#include "PhysicsHanderler.h"

//#include "RigidBody.h"

#include "TimeKeeping.h"

#include "MovingEntity.h"
#include "BlockObject.h"


#include <iostream>

PhysicsHanderler::PhysicsHanderler(TimeKeeping* tKeeping)
	: timeKeeping(tKeeping)
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
}
