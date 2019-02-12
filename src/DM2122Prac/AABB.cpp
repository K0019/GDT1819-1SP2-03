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

// Check for collision against another collider
//bool AABB::collide(const Collider& other) const
//{
//	switch (other.getType())
//	{
//	case Collider::Type::AABB:
//	{
//		const AABB* convertedOther = dynamic_cast<const AABB*>(&other);
//		if (convertedOther)
//		{
//			return (((corner1.x >= convertedOther->corner1.x && corner1.x <= convertedOther->corner2.x) || (corner2.x >= convertedOther->corner1.x && corner2.x <= convertedOther->corner2.x)) &&
//					((corner1.y >= convertedOther->corner1.y && corner1.y <= convertedOther->corner2.y) || (corner2.y >= convertedOther->corner1.y && corner2.y <= convertedOther->corner2.y)) &&
//					((corner1.z >= convertedOther->corner1.z && corner1.z <= convertedOther->corner2.z) || (corner2.z >= convertedOther->corner1.z && corner2.z <= convertedOther->corner2.z)));
//		}
//		else
//		{
//			throw std::exception("Failed to convert collider to AABB - Invalid type?");
//		}
//		throw std::exception("Control should never reach here");
//	}
//	case Collider::Type::OBB:
//	{
//		Vector3 distance = convertedOther->position - position;
//		return !( // 
//			distance.Dot(axisX) > halfX + convertedOther->halfX * convertedOther->axisX.Dot(axisX) + convertedOther->halfY * convertedOther->axisY.Dot(axisX) + convertedOther->halfZ * convertedOther->axisZ.Dot(axisX) // SA = AaxisX
//			|| distance.Dot(axisY) > halfY + convertedOther->halfX * convertedOther->axisX.Dot(axisY) + convertedOther->halfY * convertedOther->axisY.Dot(axisY) + convertedOther->halfZ * convertedOther->axisZ.Dot(axisY) // SA = AaxisY
//			|| distance.Dot(axisZ) > halfZ + convertedOther->halfX * convertedOther->axisX.Dot(axisZ) + convertedOther->halfY * convertedOther->axisY.Dot(axisZ) + convertedOther->halfZ * convertedOther->axisZ.Dot(axisZ) // SA = AaxisZ
//			|| distance.Dot(convertedOther->axisX) > halfX * axisX.Dot(convertedOther->axisX) + halfY * axisY.Dot(convertedOther->axisX) + halfZ * axisZ.Dot(convertedOther->axisX) + convertedOther->halfX // SA = BaxisX
//			|| distance.Dot(convertedOther->axisY) > halfX * axisX.Dot(convertedOther->axisY) + halfY * axisY.Dot(convertedOther->axisY) + halfZ * axisZ.Dot(convertedOther->axisY) + convertedOther->halfY // SA = BaxisY
//			|| distance.Dot(convertedOther->axisZ) > halfX * axisX.Dot(convertedOther->axisZ) + halfY * axisY.Dot(convertedOther->axisZ) + halfZ * axisZ.Dot(convertedOther->axisZ) + convertedOther->halfZ // SA = BaxisZ
//			|| distance.Dot(axisX.Cross(convertedOther->axisX)) > halfY * axisZ.Dot(convertedOther->axisX) + halfZ * axisY.Dot(convertedOther->axisX) + convertedOther->halfY * axisX.Dot(convertedOther->axisZ) + convertedOther->halfZ * axisX.Dot(convertedOther->axisY) // SP = AaxisX x BaxisX
//			|| distance.Dot(axisX.Cross(convertedOther->axisY)) > halfY * axisZ.Dot(convertedOther->axisY) + halfZ * axisY.Dot(convertedOther->axisY) + convertedOther->halfX * axisX.Dot(convertedOther->axisZ) + convertedOther->halfZ * axisX.Dot(convertedOther->axisX) // SP = AaxisX x BaxisY
//			|| distance.Dot(axisX.Cross(convertedOther->axisZ)) > halfY * axisZ.Dot(convertedOther->axisZ) + halfZ * axisY.Dot(convertedOther->axisZ) + convertedOther->halfX * axisX.Dot(convertedOther->axisY) + convertedOther->halfY * axisX.Dot(convertedOther->axisX) // SP = AaxisX x BaxisZ
//			|| distance.Dot(axisY.Cross(convertedOther->axisX)) > halfX * axisZ.Dot(convertedOther->axisX) + halfZ * axisX.Dot(convertedOther->axisX) + convertedOther->halfY * axisY.Dot(convertedOther->axisZ) + convertedOther->halfZ * axisY.Dot(convertedOther->axisY) // SP = AaxisY x BaxisX
//			|| distance.Dot(axisY.Cross(convertedOther->axisY)) > halfX * axisZ.Dot(convertedOther->axisY) + halfZ * axisX.Dot(convertedOther->axisY) + convertedOther->halfX * axisY.Dot(convertedOther->axisZ) + convertedOther->halfZ * axisY.Dot(convertedOther->axisX) // SP = AaxisY x BaxisY
//			|| distance.Dot(axisY.Cross(convertedOther->axisZ)) > halfX * axisZ.Dot(convertedOther->axisZ) + halfZ * axisX.Dot(convertedOther->axisZ) + convertedOther->halfX * axisY.Dot(convertedOther->axisY) + convertedOther->halfY * axisY.Dot(convertedOther->axisX) // SP = AaxisY x BaxisZ
//			|| distance.Dot(axisZ.Cross(convertedOther->axisX)) > halfX * axisY.Dot(convertedOther->axisX) + halfY * axisX.Dot(convertedOther->axisX) + convertedOther->halfY * axisZ.Dot(convertedOther->axisZ) + convertedOther->halfZ * axisZ.Dot(convertedOther->axisY) // SP = AaxisZ x BaxisX
//			|| distance.Dot(axisZ.Cross(convertedOther->axisY)) > halfX * axisY.Dot(convertedOther->axisY) + halfY * axisX.Dot(convertedOther->axisY) + convertedOther->halfX * axisZ.Dot(convertedOther->axisZ) + convertedOther->halfZ * axisZ.Dot(convertedOther->axisX) // SP = AaxisZ x BaxisY
//			|| distance.Dot(axisZ.Cross(convertedOther->axisZ)) > halfX * axisY.Dot(convertedOther->axisZ) + halfY * axisX.Dot(convertedOther->axisZ) + convertedOther->halfX * axisZ.Dot(convertedOther->axisY) + convertedOther->halfY * axisZ.Dot(convertedOther->axisX) // SP = AaxisZ x BaxisZ
//			);
//	}
//	default:
//		throw std::exception("Unimplemented collider type");
//	}
//}