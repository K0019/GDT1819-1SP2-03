#ifndef AABB_H
#define AABB_H
#include "Vector3.h"
#include "MeshPlaceable.h"

/* File: AABB.h
   Use: Collider used in detecting collision when placing objects */

class AABB
{
public:
	AABB(const Vector3& corner1, const Vector3& corner2); // Constructor
	~AABB(); // Destructor

	bool collide(const AABB& other) const; // Check for collision against another AABB
protected:

private:
	Vector3 corner1, corner2; // AABB data
};

#endif // !AABB_H
