#include "OBB.h"

OBB::OBB(const Vector3& position, const Vector3& axisX, const Vector3& axisY, const Vector3& axisZ, float halfX, float halfY, float halfZ)
	: Collider(Collider::Type::OBB)
	, position(position)
	, axisX(axisX)
	, axisY(axisY)
	, axisZ(axisZ)
	, halfX(halfX)
	, halfY(halfY)
	, halfZ(halfZ)
{

}

OBB::~OBB()
{

}

bool OBB::collide(const Collider& other) const
{
	switch (other.getType())
	{
	case Collider::Type::AABB:
		throw std::exception("OBB undefined against AABB");
	case Collider::Type::OBB:
	{
		const OBB* convertedOther = dynamic_cast<const OBB*>(&other);
		if (convertedOther)
		{
			Vector3 distance = convertedOther->position - position;
			return !( // 
				distance.Dot(axisX) > halfX + convertedOther->halfX * convertedOther->axisX.Dot(axisX) + convertedOther->halfY * convertedOther->axisY.Dot(axisX) + convertedOther->halfZ * convertedOther->axisZ.Dot(axisX) // SA = AaxisX
				|| distance.Dot(axisY) > halfY + convertedOther->halfX * convertedOther->axisX.Dot(axisY) + convertedOther->halfY * convertedOther->axisY.Dot(axisY) + convertedOther->halfZ * convertedOther->axisZ.Dot(axisY) // SA = AaxisY
				|| distance.Dot(axisZ) > halfZ + convertedOther->halfX * convertedOther->axisX.Dot(axisZ) + convertedOther->halfY * convertedOther->axisY.Dot(axisZ) + convertedOther->halfZ * convertedOther->axisZ.Dot(axisZ) // SA = AaxisZ
				|| distance.Dot(convertedOther->axisX) > halfX * axisX.Dot(convertedOther->axisX) + halfY * axisY.Dot(convertedOther->axisX) + halfZ * axisZ.Dot(convertedOther->axisX) + convertedOther->halfX // SA = BaxisX
				|| distance.Dot(convertedOther->axisY) > halfX * axisX.Dot(convertedOther->axisY) + halfY * axisY.Dot(convertedOther->axisY) + halfZ * axisZ.Dot(convertedOther->axisY) + convertedOther->halfY // SA = BaxisY
				|| distance.Dot(convertedOther->axisZ) > halfX * axisX.Dot(convertedOther->axisZ) + halfY * axisY.Dot(convertedOther->axisZ) + halfZ * axisZ.Dot(convertedOther->axisZ) + convertedOther->halfZ // SA = BaxisZ
				|| distance.Dot(axisX.Cross(convertedOther->axisX)) > halfY * axisZ.Dot(convertedOther->axisX) + halfZ * axisY.Dot(convertedOther->axisX) + convertedOther->halfY * axisX.Dot(convertedOther->axisZ) + convertedOther->halfZ * axisX.Dot(convertedOther->axisY) // SP = AaxisX x BaxisX
				|| distance.Dot(axisX.Cross(convertedOther->axisY)) > halfY * axisZ.Dot(convertedOther->axisY) + halfZ * axisY.Dot(convertedOther->axisY) + convertedOther->halfX * axisX.Dot(convertedOther->axisZ) + convertedOther->halfZ * axisX.Dot(convertedOther->axisX) // SP = AaxisX x BaxisY
				|| distance.Dot(axisX.Cross(convertedOther->axisZ)) > halfY * axisZ.Dot(convertedOther->axisZ) + halfZ * axisY.Dot(convertedOther->axisZ) + convertedOther->halfX * axisX.Dot(convertedOther->axisY) + convertedOther->halfY * axisX.Dot(convertedOther->axisX) // SP = AaxisX x BaxisZ
				|| distance.Dot(axisY.Cross(convertedOther->axisX)) > halfX * axisZ.Dot(convertedOther->axisX) + halfZ * axisX.Dot(convertedOther->axisX) + convertedOther->halfY * axisY.Dot(convertedOther->axisZ) + convertedOther->halfZ * axisY.Dot(convertedOther->axisY) // SP = AaxisY x BaxisX
				|| distance.Dot(axisY.Cross(convertedOther->axisY)) > halfX * axisZ.Dot(convertedOther->axisY) + halfZ * axisX.Dot(convertedOther->axisY) + convertedOther->halfX * axisY.Dot(convertedOther->axisZ) + convertedOther->halfZ * axisY.Dot(convertedOther->axisX) // SP = AaxisY x BaxisY
				|| distance.Dot(axisY.Cross(convertedOther->axisZ)) > halfX * axisZ.Dot(convertedOther->axisZ) + halfZ * axisX.Dot(convertedOther->axisZ) + convertedOther->halfX * axisY.Dot(convertedOther->axisY) + convertedOther->halfY * axisY.Dot(convertedOther->axisX) // SP = AaxisY x BaxisZ
				|| distance.Dot(axisZ.Cross(convertedOther->axisX)) > halfX * axisY.Dot(convertedOther->axisX) + halfY * axisX.Dot(convertedOther->axisX) + convertedOther->halfY * axisZ.Dot(convertedOther->axisZ) + convertedOther->halfZ * axisZ.Dot(convertedOther->axisY) // SP = AaxisZ x BaxisX
				|| distance.Dot(axisZ.Cross(convertedOther->axisY)) > halfX * axisY.Dot(convertedOther->axisY) + halfY * axisX.Dot(convertedOther->axisY) + convertedOther->halfX * axisZ.Dot(convertedOther->axisZ) + convertedOther->halfZ * axisZ.Dot(convertedOther->axisX) // SP = AaxisZ x BaxisY
				|| distance.Dot(axisZ.Cross(convertedOther->axisZ)) > halfX * axisY.Dot(convertedOther->axisZ) + halfY * axisX.Dot(convertedOther->axisZ) + convertedOther->halfX * axisZ.Dot(convertedOther->axisY) + convertedOther->halfY * axisZ.Dot(convertedOther->axisX) // SP = AaxisZ x BaxisZ
				);
		}
		else
		{
			throw std::exception("Unable to convert collider to OBB - Invalid type?");
		}
		throw std::exception("Control should never reach here");
	}
	default:
		throw std::exception("Unimplemented collider type");
	}
}