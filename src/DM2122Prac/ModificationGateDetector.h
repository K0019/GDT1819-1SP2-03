#ifndef MODIFICATIONGATEDETECTOR_H
#define MODIFICATIONGATEDETECTOR_H
#include <vector>
#include "Object.h"
#include "Kart.h"
#include "CollisionChecker.h"

class ModificationGateDetector
{
public:
	ModificationGateDetector();
	~ModificationGateDetector();

	void update();

	void registerKart(Kart* kart);
	void registerGate(const Object* gate);

	void removeKart(Kart* kart);
	void removeGate(const Object* gate);
protected:

private:
	std::vector<Kart*> karts;
	std::vector<bool> changed;
	std::vector<const Object*> objects;
};

namespace ModGate
{
	extern ModificationGateDetector detector;
}

#endif // !MODIFICATIONGATEDETECTOR_H
