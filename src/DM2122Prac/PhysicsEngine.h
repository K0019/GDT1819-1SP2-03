#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H
#include <vector>
#include <algorithm>
#include "StaticPhysicsObjectInternal.h"
#include "MovingPhysicsObjectInternal.h"
#include "PackedCollision.h"

class PhysicsEngine
{
public:
	PhysicsEngine(); // Constructor
	~PhysicsEngine(); // Destructor

	void registerMovingObject(MovingPhysicsObjectInternal* object);
	void registerStaticObject(StaticPhysicsObjectInternal* object);
	void removeMovingObject(MovingPhysicsObjectInternal* object);
	void removeStaticObject(StaticPhysicsObjectInternal* object);

	void update();

	void empty();
protected:

private:
	void updateHorizontal();

	static int instances;

	std::vector<MovingPhysicsObjectInternal*> movingObjects;
	std::vector<StaticPhysicsObjectInternal*> staticObjects;
};

namespace Physics
{
	extern PhysicsEngine physicsEngine;
}

#endif // !PHYSICSENGINE_H
