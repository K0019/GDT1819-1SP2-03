#ifndef MODIFICATIONGATE_H
#define MODIFICATIONGATE_H
#include "Object.h"
#include "ModificationGateDetector.h"

class ModificationGate : public Object
{
public:
	ModificationGate(MeshPlaceable* mesh, int gridX, int gridZ, Rotation rotation);
	~ModificationGate();
protected:

private:
};

#endif // !MODIFICATIONGATE_H
