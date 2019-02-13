#include "StaticPhysicsObject.h"

StaticPhysicsObject::StaticPhysicsObject(const AABB& box)
	: StaticPhysicsObjectInternal(box)
{
	Physics::physicsEngine.registerStaticObject(this);
}

StaticPhysicsObject::~StaticPhysicsObject()
{
	Physics::physicsEngine.removeStaticObject(this);
}