#ifndef COLLISIONINFO
#define COLLISIONINFO
#include "Vector3.h"

class CollisionInfo
{
public:
	explicit CollisionInfo(bool isColliding);
	CollisionInfo(bool isColliding, const Vector3& contactNormal, float penetrationDistance);
	~CollisionInfo();

	bool isColliding() const;
	const Vector3& getContactNormal() const;
	float getDistance() const;

	void recalculateDistance(const Vector3& movement);

	operator bool() const;
protected:

private:
	bool penetrating;

	Vector3 contactNormal;
	float penetrationDistance;
};

#endif // !COLLISIONINFO
