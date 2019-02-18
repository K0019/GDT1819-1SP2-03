#include "CollisionInfo.h"

CollisionInfo::CollisionInfo(bool isColliding)
	: penetrating(isColliding)
{

}

CollisionInfo::CollisionInfo(bool isColliding, const Vector3& contactNormal, float penetrationDistance)
	: penetrating(isColliding)
	, contactNormal(contactNormal)
	, penetrationDistance(penetrationDistance)
{

}

CollisionInfo::~CollisionInfo()
{

}

bool CollisionInfo::isColliding() const
{
	return penetrating;
}

const Vector3& CollisionInfo::getContactNormal() const
{
	return contactNormal;
}

float CollisionInfo::getDistance() const
{
	return penetrationDistance;
}

void CollisionInfo::recalculateDistance(const Vector3& movement)
{
	penetrationDistance -= movement.Dot(contactNormal);
}

CollisionInfo::operator bool() const
{
	return penetrating;
}