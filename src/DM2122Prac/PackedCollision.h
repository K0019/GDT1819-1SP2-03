#ifndef PACKEDCOLLISION_H
#define PACKEDCOLLISION_H
#include "CollisionInfo.h"
#include "Collider.h"
#include "CollisionChecker.h"

class PackedCollision
{
public:
	PackedCollision(const CollisionInfo& info, const Collider* lhs, const Collider* rhs);
	~PackedCollision();

	const CollisionInfo& getInfo() const;
	const Collider* getLhs() const;
	const Collider* getRhs() const;

	void updateInfo(const Vector3& movement);
	void recalculateCollision();
protected:

private:
	CollisionInfo info;
	const Collider* lhs, *rhs;
};

#endif // !PACKEDCOLLISION_H
