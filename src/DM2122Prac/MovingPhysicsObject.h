#ifndef MOVINGPHYSICSOBJECT_H
#define MOVINGPHYSICSOBJECT_H
#include "MovingPhysicsObjectInternal.h"
#include "PhysicsEngine.h"

class MovingPhysicsObject : public MovingPhysicsObjectInternal
{
public:
	MovingPhysicsObject(const OBB& box);
	~MovingPhysicsObject();

protected:

private:
};

#endif // !MOVINGPHYSICSOBJECT_H
