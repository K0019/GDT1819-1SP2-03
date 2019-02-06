#include "AABB.h"

// Constructor
AABB::AABB(const Vector3& corner1, const Vector3& corner2)
	: corner1(corner1)
	, corner2(corner2)
{

}

// Destructor
AABB::~AABB()
{

}

// Check for collision against another AABB
bool AABB::collide(const AABB& other) const
{
	return (((corner1.x >= other.corner1.x && corner1.x <= other.corner2.x) || (corner2.x >= other.corner1.x && corner2.x <= other.corner2.x)) &&
			((corner1.y >= other.corner1.y && corner1.y <= other.corner2.y) || (corner2.y >= other.corner1.y && corner2.y <= other.corner2.y)) &&
			((corner1.z >= other.corner1.z && corner1.z <= other.corner2.z) || (corner2.z >= other.corner1.z && corner2.z <= other.corner2.z)));
}