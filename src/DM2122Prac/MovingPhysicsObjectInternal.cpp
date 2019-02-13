#include "MovingPhysicsObjectInternal.h"

MovingPhysicsObjectInternal::MovingPhysicsObjectInternal(const OBB& box)
	: collisionBox(box)
{

}

MovingPhysicsObjectInternal::~MovingPhysicsObjectInternal()
{

}

const OBB& MovingPhysicsObjectInternal::getCollisionBox() const
{
	return collisionBox;
}

void MovingPhysicsObjectInternal::setCollisionPosition(const Vector3& position)
{
	collisionBox.setPosition(position);
}