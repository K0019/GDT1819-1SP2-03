#include "StaticPhysicsObject.h"

StaticPhysicsObject::StaticPhysicsObject(const AABB& box, bool physicsEnabled)
	: StaticPhysicsObjectInternal(box, physicsEnabled)
{
	Physics::physicsEngine.registerStaticObject(this);
}

StaticPhysicsObject::~StaticPhysicsObject()
{
	Physics::physicsEngine.removeStaticObject(this);
}