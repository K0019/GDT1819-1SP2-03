#ifndef OBJECT_H
#define OBJECT_H
#include "MatrixStack.h"
#include "MeshPlaceable.h"
#include "AABB.h"

/* File: Object.h
   Use: Class containing the data of an instance of a placed meshPlaceable */

class Object
{
public:
	enum Rotation // Orientation of the object with respect to world space
	{
		NORTH = 0,
		EAST,
		SOUTH,
		WEST
	};
	Object(MeshPlaceable* /*meshunsigned int ID*/, int gridX, int gridZ, Rotation rotation); // Constructor
	~Object(); // Destructor

	void render(unsigned int uMatrixMVS) const; // Render this object

	const AABB& getAABB() const; // Retrieve AABB collider for this object

	static AABB createAABB(const MeshPlaceable* mesh, int gridX, int gridZ, Object::Rotation rotation); // Generate an AABB of a meshPlaceable
	static AABB createAABB(int lengthX, int lengthY, int lengthZ, int gridX, int gridY, int gridZ, Object::Rotation rotation); // Generate a custom AABB
	void setID(unsigned int);
	int getGridX() const;
	int getGridY() const;
	int getGridZ() const;
	int getrotation() const;
	int getID() const;
protected:

private:
	MeshPlaceable* mesh; // Mesh that this object is of

	int gridX, gridZ, gridY; // Coordinates in grid space
	unsigned ID;
	Rotation rotation; // Rotation of this object

	AABB gridOccupation; // AABB collider of this object
};

Object::Rotation& operator++(Object::Rotation& rotation); // Rotate a rotation clockwise

#endif // !OBJECT_H