#include "PhysicsHanderler.h"

//#include "RigidBody.h"

#include "TimeKeeping.h"

#include "MovingEntity.h"
#include "BlockObject.h"


PhysicsHanderler::PhysicsHanderler(TimeKeeping* tKeeping)
	: timeKeeping(tKeeping)
{

}

void PhysicsHanderler::RegisterRigidBody(MovingEntity* rigidBody)
{
	movingEntities.push_back(rigidBody);
}

void PhysicsHanderler::TickPhysics()
{
	float deltaForThisFrame = timeKeeping->GetDeltaTime();

	for (auto movingEntity : movingEntities) {
		movingEntity->TickThisRigidBodyPhysics(deltaForThisFrame); // move the object.

		// check for collision with all blocks. Can be optimised to use only blocks within radius.
		for (auto staticObject : blocks){
			if (movingEntity->CheckForCollision(staticObject->GetBounds(), staticObject->transform.position)) {
				// collision detected!
				DirectX::XMVECTOR overlapAmount = movingEntity->GetOverlapAmount(staticObject->GetBounds(), staticObject->transform.position);
				// do something to the velocity.

				// Move the entity back.
				DirectX::XMVECTOR pos = movingEntity->transform.position;

				movingEntity->transform.position = DirectX::XMVectorSet(DirectX::XMVectorGetX(pos) + DirectX::XMVectorGetX(overlapAmount), 
					DirectX::XMVectorGetY(pos) + DirectX::XMVectorGetY(overlapAmount), DirectX::XMVectorGetZ(pos) + DirectX::XMVectorGetZ(overlapAmount), 0);
			}
		}
	}
}
