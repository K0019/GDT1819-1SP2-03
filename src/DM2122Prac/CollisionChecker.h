#ifndef COLLISIONCHECKER_H
#define COLLISIONCHECKER_H
#include "AABB.h"
#include "OBB.h"
#include "Triangle.h"
#include "CollisionInfo.h"

class CollisionChecker
{
public:
	static CollisionInfo collide(const Collider& lhs, const Collider& rhs);

protected:

private:
	CollisionChecker();

	static CollisionInfo collide(const AABB* lhs, const AABB* rhs);
	static CollisionInfo collide(const AABB* lhs, const OBB* rhs);
	static CollisionInfo collide(const OBB* lhs, const OBB* rhs);
	static CollisionInfo collide(const OBB* lhs, const Triangle* rhs);
};

#endif // !COLLISIONCHECKER_H
