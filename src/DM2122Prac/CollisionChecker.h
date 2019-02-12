#ifndef COLLISIONCHECKER_H
#define COLLISIONCHECKER_H
#include "AABB.h"
#include "OBB.h"
#include "Triangle.h"

class CollisionChecker
{
public:
	static bool collide(const Collider& lhs, const Collider& rhs);

protected:

private:
	CollisionChecker();

	static bool collide(const AABB* lhs, const AABB* rhs);
	static bool collide(const AABB* lhs, const OBB* rhs);
	static bool collide(const OBB* lhs, const OBB* rhs);
	static bool collide(const OBB* lhs, const Triangle* rhs);
};

#endif // !COLLISIONCHECKER_H
