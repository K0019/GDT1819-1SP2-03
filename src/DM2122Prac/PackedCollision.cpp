#include "PackedCollision.h"

PackedCollision::PackedCollision(const CollisionInfo& info, const Collider* lhs, const Collider* rhs)
	: info(info)
	, lhs(lhs)
	, rhs(rhs)
{

}

PackedCollision::~PackedCollision()
{

}

const CollisionInfo& PackedCollision::getInfo() const
{
	return info;
}

const Collider* PackedCollision::getLhs() const
{
	return lhs;
}

const Collider* PackedCollision::getRhs() const
{
	return rhs;
}

void PackedCollision::updateInfo(const Vector3& movement)
{
	info.recalculateDistance(movement);
}

void PackedCollision::recalculateCollision()
{
	info = CollisionChecker::collide(*lhs, *rhs);
}