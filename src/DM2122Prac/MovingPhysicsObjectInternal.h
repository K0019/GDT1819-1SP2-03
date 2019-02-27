#ifndef MOVINGPHYSICSOBJECTINTERNAL_H
#define MOVINGPHYSICSOBJECTINTERNAL_H
#include "CollisionChecker.h"
#include "Vector3.h"

class MovingPhysicsObjectInternal
{
public:
	MovingPhysicsObjectInternal(const OBB& box);
	~MovingPhysicsObjectInternal();

	const OBB& getCollisionBox() const;
	void setCollisionPosition(const Vector3& position);

	const Vector3& getVelocity() const;

	virtual void moveObject(const Vector3& displacement) = 0;
protected:
	Vector3 velocity;

	void rotateCollision(float degree, const Vector3& axis);
private:
	OBB collisionBox;
};

#endif // !MOVINGPHYSICSOBJECTINTERNAL_H
