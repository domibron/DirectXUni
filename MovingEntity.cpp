#include "MovingEntity.h"

MovingEntity::MovingEntity(bool useGravity, float cXPos, float cYPos, float cZPos, float cXSize, float cYSize, float cZSize)
	: RigidBody(&transform, useGravity, cXPos, cYPos, cZPos, cXSize, cYSize, cZSize)
{
	
}
