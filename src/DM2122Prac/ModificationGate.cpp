#include "ModificationGate.h"

ModificationGate::ModificationGate(MeshPlaceable* mesh, int gridX, int gridZ, Rotation rotation)
	: Object(mesh, gridX, gridZ, rotation)
{
	ModGate::detector.registerGate(this);
}

ModificationGate::~ModificationGate()
{
	ModGate::detector.removeGate(this);
}

