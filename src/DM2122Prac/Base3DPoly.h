#pragma once
#ifndef BASE3DPOLY_H
#define BASE3DPOLY_H
#include "Source/MeshBuilder.h"

/* File: Base3DPoly
   Use: Base class of a single mesh object 
   
   Note: Complex objects such as Bear and TreeWinter cannot use Base3DPoly, only single mesh objects such as Lamp */

class Base3DPoly
{
public:
	explicit Base3DPoly(Mesh* const mesh); // Constructor
	~Base3DPoly(); // Destructor

	void draw() const; // Draw the mesh assigned to this object
	void setColor(const Vector3& color); // Set the color of the mesh, fixed attenuation
	void setColor(unsigned short r, unsigned short g, unsigned short b); // Set the color of the mesh, fixed attenuation

protected:
	Mesh* mesh; // Mesh
};

#endif // !BASE3DPOLY_H
