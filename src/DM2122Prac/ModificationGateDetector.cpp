#include "ModificationGateDetector.h"

ModificationGateDetector ModGate::detector = ModificationGateDetector();

ModificationGateDetector::ModificationGateDetector()
{

}

ModificationGateDetector::~ModificationGateDetector()
{

}

void ModificationGateDetector::update()
{
	int index = 0;
	for (auto& k : karts)
	{
		bool detected = false;
		for (auto& o : objects)
		{
			if (CollisionChecker::collide(k->getCollisionBox(), o->getAABB()))
			{
				detected = true;
				if (!changed[index])
				{
					k->changeStatus();
				}
				break;
			}
		}

		changed[index++] = detected;
	}
}

void ModificationGateDetector::registerKart(Kart* kart)
{
	karts.push_back(kart);
	changed.push_back(false);
}

void ModificationGateDetector::registerGate(const Object* gate)
{
	objects.push_back(gate);
}

void ModificationGateDetector::removeKart(Kart* kart)
{
	int i = 0;
	for (std::vector<Kart*>::const_iterator k = karts.begin(); k != karts.end(); ++k)
	{
		if (*k == kart)
		{
			karts.erase(k);
			changed.erase(changed.begin() + i);
			break;
		}
		++i;
	}
}

void ModificationGateDetector::removeGate(const Object* gate)
{
	for (std::vector<const Object*>::const_iterator o = objects.begin(); o != objects.end(); ++o)
	{
		if (*o == gate)
		{
			objects.erase(o);
			break;
		}
	}
}