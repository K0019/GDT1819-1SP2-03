#ifndef AABB_H
#define AABB_H
#include "Collider.h"
#include "Vector3.h"
#include "MeshPlaceable.h"

/* File: AABB.h
   Use: Collider used in detecting collision when placing objects */

class AABB : public Collider
{
public:
	AABB(const Vector3& corner1, const Vector3& corner2); // Constructor
	~AABB(); // Destructor

	virtual bool collide(const Collider& other) const override; // Check for collision against another collider
protected:

private:
	Vector3 corner1, corner2; // AABB data
};

#endif // !AABB_H
