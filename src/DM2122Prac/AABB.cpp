#include "AABB.h"

// Constructor
AABB::AABB(const Vector3& corner1, const Vector3& corner2)
	: Collider(Collider::Type::AABB)
	, corner1(corner1)
	, corner2(corner2)
{

}

// Destructor
AABB::~AABB()
{

}

const Vector3& AABB::getCorner1() const
{
	return corner1;
}
const Vector3& AABB::getCorner2() const
{
	return corner2;
}