#pragma once
#include <vector>

class BlockObject;
class MovingEntity;
class TimeKeeping;

class PhysicsHanderler
{
private:
	std::vector<BlockObject*> blocks;
	std::vector<MovingEntity*> movingEntities;
	TimeKeeping* timeKeeping;

public:
	PhysicsHanderler(TimeKeeping* tkeeping);
	void RegisterStaticBody(BlockObject* staticObject); // just a raw collider.
	void RegisterRigidBody(MovingEntity* rigidBody); // a moveable collider.

	void RemoveStaticBody(BlockObject* staticObject);
	void RemoveRigidBody(MovingEntity* rigidBody);

	void TickPhysics();
};

