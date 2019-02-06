#include "Base3DPoly.h"

// Constructor
Base3DPoly::Base3DPoly(Mesh* const mesh)
	: mesh(mesh)
{
}

// Destructor
Base3DPoly::~Base3DPoly()
{
	delete mesh;
}

// Render the mesh assigned to this object
void Base3DPoly::draw() const
{
	// Call mesh to render
	mesh->Render();
}

// Set the color of the mesh, fixed attenuation
void Base3DPoly::setColor(const Vector3& color)
{
	// Call mesh to change its color, with preset material configuration
	mesh->setColor(color, 0.4f, 0.5f, 0.05f, 32);
}

// Set the color of the mesh, fixed attenuation
void Base3DPoly::setColor(unsigned short r, unsigned short g, unsigned short b)
{
	// Call mesh to change its color, with preset material configuration
	mesh->setColor({ static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f }, 0.4f, 0.5f, 0.05f, 32);
}