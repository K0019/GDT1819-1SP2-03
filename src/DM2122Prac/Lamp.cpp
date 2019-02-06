#include "Lamp.h"

// Constructor
Lamp::Lamp(const unsigned int uColorData)
	: Base3DPoly(MeshBuilder::GenerateSphere(1.0f, uColorData, true)) // Generate sphere
{
}

// Destructor
Lamp::~Lamp()
{
}