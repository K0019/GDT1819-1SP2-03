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

// Check for collision against another collider
bool AABB::collide(const Collider& other) const
{
	switch (other.getType())
	{
	case Collider::Type::AABB:
	{
		const AABB* convertedOther = dynamic_cast<const AABB*>(&other);
		if (convertedOther)
		{
			return (((corner1.x >= convertedOther->corner1.x && corner1.x <= convertedOther->corner2.x) || (corner2.x >= convertedOther->corner1.x && corner2.x <= convertedOther->corner2.x)) &&
					((corner1.y >= convertedOther->corner1.y && corner1.y <= convertedOther->corner2.y) || (corner2.y >= convertedOther->corner1.y && corner2.y <= convertedOther->corner2.y)) &&
					((corner1.z >= convertedOther->corner1.z && corner1.z <= convertedOther->corner2.z) || (corner2.z >= convertedOther->corner1.z && corner2.z <= convertedOther->corner2.z)));
		}
		else
		{
			throw std::exception("Failed to convert collider to AABB - Invalid type?");
		}
		throw std::exception("Control should never reach here");
	}
	case Collider::Type::OBB:
		throw std::exception("AABB undefined against OBB");
	default:
		throw std::exception("Unimplemented collider type");
	}
}