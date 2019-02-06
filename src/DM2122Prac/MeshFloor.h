#ifndef MESHFLOOR_H
#define MESHFLOOR_H
#include "Source/Mesh.h"
#include "MatrixStack.h"

/* File: MeshFloor.h
   Use: Mesh specifically for the floor. Renders 9 times in a grid around the player position, effectively making this mesh infinite */

class MeshFloor
{
public:
	MeshFloor(Mesh* mesh); // Constructor
	virtual ~MeshFloor(); // Destructor

	void render(const Vector3& playerPos, unsigned int uMatrixMVS) const; // Render (9 times, at and around the player)

protected:

private:
	Mesh* mesh; // Textured horizontal quad mesh
};

#endif // !MESHFLOOR_H
