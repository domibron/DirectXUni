#pragma once
#include <vector>

#include "DirectXMath.h"

class BlockObject;
class MovingEntity;
class TimeKeeping;
class Transform;

class PhysicsHanderler
{
private:
	std::vector<BlockObject*> blocks;
	std::vector<MovingEntity*> movingEntities;
	TimeKeeping* timeKeeping;
	Transform* camTransform;


public:
	PhysicsHanderler(TimeKeeping* tkeeping, Transform* cameraTransform);
	void RegisterStaticBody(BlockObject* staticObject); // just a raw collider.
	void RegisterRigidBody(MovingEntity* rigidBody); // a moveable collider.

	void RemoveStaticBody(BlockObject* staticObject);
	void RemoveRigidBody(MovingEntity* rigidBody);

	void TickPhysics();

	BlockObject* slectedBlock = nullptr;
	DirectX::XMVECTOR hitPoint;
	DirectX::XMVECTOR normal;
};

