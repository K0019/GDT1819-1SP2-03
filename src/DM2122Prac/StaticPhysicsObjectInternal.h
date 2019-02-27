#ifndef STATICPHYSICSOBJECTINTERNAL_H
#define STATICPHYSICSOBJECTINTERNAL_H
#include "CollisionChecker.h"

class StaticPhysicsObjectInternal
{
public:
	StaticPhysicsObjectInternal(const AABB& box, bool physicsEnabled);
	~StaticPhysicsObjectInternal();

	const AABB& getAABB() const;
	const std::vector<Triangle>& getTriangles() const;
	bool getPhysicsEnabled() const;
protected:
	void initTriangles(const std::vector<Triangle>& triangles);

private:
	bool physicsEnabled;

	AABB broadBox;
	std::vector<Triangle> triangles;
};

#endif // !STATICPHYSICSOBJECTINTERNAL_H
