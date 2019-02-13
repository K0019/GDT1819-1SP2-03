#ifndef MOVINGPHYSICSOBJECTINTERNAL_H
#define MOVINGPHYSICSOBJECTINTERNAL_H
#include "PhysicsObject.h"

class MovingPhysicsObjectInternal : public PhysicsObject
{
public:
	MovingPhysicsObjectInternal(const OBB& box);
	~MovingPhysicsObjectInternal();

	const OBB& getCollisionBox() const;
	void setCollisionPosition(const Vector3& position);
protected:

private:
	OBB collisionBox;
};

#endif // !MOVINGPHYSICSOBJECTINTERNAL_H
