#include "ModificationGateDetector.h"

ModificationGateDetector ModGate::detector = ModificationGateDetector();

ModificationGateDetector::ModificationGateDetector()
	: objects(nullptr)
{

}

ModificationGateDetector::~ModificationGateDetector()
{

}

void ModificationGateDetector::update()
{
	if (objects != nullptr)
	{
		int index = 0;
		for (auto& k : karts)
		{
			bool detected = false;
			for (auto& o : *objects)
			{
				if (o->getID() == 4 && CollisionChecker::collide(k->getCollisionBox(), o->getAABB()))
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
}

void ModificationGateDetector::registerKart(Kart* kart)
{
	karts.push_back(kart);
	changed.push_back(false);
}

void ModificationGateDetector::registerObjects(std::vector<Object*>* objects)
{
	this->objects = objects;
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

void ModificationGateDetector::removeObjects()
{
	objects = nullptr;
}