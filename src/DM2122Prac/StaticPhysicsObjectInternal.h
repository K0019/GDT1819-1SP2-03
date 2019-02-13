#ifndef STATICPHYSICSOBJECTINTERNAL_H
#define STATICPHYSICSOBJECTINTERNAL_H
#include "PhysicsObject.h"

class StaticPhysicsObjectInternal : public PhysicsObject
{
public:
	StaticPhysicsObjectInternal(const AABB& box);
	~StaticPhysicsObjectInternal();

	const AABB& getAABB() const;
	const std::vector<Triangle>& getTriangles() const;
protected:
	void initTriangles(const std::vector<Triangle>& triangles);

private:
	AABB broadBox;
	std::vector<Triangle> triangles;
};

#endif // !STATICPHYSICSOBJECTINTERNAL_H
