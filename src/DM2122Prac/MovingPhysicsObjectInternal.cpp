#include "MovingPhysicsObjectInternal.h"

MovingPhysicsObjectInternal::MovingPhysicsObjectInternal(const OBB& box)
	: collisionBox(box)
	, velocity(Vector3())
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

void MovingPhysicsObjectInternal::rotateCollision(float degree, const Vector3& axis)
{
	collisionBox.rotate(degree, axis);
}

const Vector3& MovingPhysicsObjectInternal::getVelocity() const
{
	return velocity;
}