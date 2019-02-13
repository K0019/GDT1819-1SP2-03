#include "MovingPhysicsObject.h"

MovingPhysicsObject::MovingPhysicsObject(const OBB& box)
	: MovingPhysicsObjectInternal(box)
{
	Physics::physicsEngine.registerMovingObject(this);
}

MovingPhysicsObject::~MovingPhysicsObject()
{
	Physics::physicsEngine.removeMovingObject(this);
}