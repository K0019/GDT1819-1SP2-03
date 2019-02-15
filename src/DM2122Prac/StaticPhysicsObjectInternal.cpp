#include "StaticPhysicsObjectInternal.h"

StaticPhysicsObjectInternal::StaticPhysicsObjectInternal(const AABB& box, bool physicsEnabled)
	: broadBox(box)
	, physicsEnabled(physicsEnabled)
{

}

StaticPhysicsObjectInternal::~StaticPhysicsObjectInternal()
{

}

const AABB& StaticPhysicsObjectInternal::getAABB() const
{
	return broadBox;
}

const std::vector<Triangle>& StaticPhysicsObjectInternal::getTriangles() const
{
	return triangles;
}

bool StaticPhysicsObjectInternal::getPhysicsEnabled() const
{
	return physicsEnabled;
}

void StaticPhysicsObjectInternal::initTriangles(const std::vector<Triangle>& triangles)
{
	if (this->triangles.size() != 0)
	{
		throw std::exception("Tried to re-initialize triangles");
	}

	this->triangles = triangles;
}