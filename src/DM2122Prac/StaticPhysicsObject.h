#ifndef STATICPHYSICSOBJECT_H
#define STATICPHYSICSOBJECT_H
#include "StaticPhysicsObjectInternal.h"
#include "PhysicsEngine.h"

class StaticPhysicsObject : public StaticPhysicsObjectInternal
{
public:
	StaticPhysicsObject(const AABB& box);
	~StaticPhysicsObject();

protected:

private:
};

#endif // !STATICPHYSICSOBJECT_H
