#include "HandleLap.h"

HandleLap::HandleLap(const ObjectList* objectList)
	: objectList(objectList)
{

}

HandleLap::HandleLap(const ObjectList* objectList, std::initializer_list<const Kart*> karts)
	: objectList(objectList)
{
	for (auto& k : karts)
	{
		this->karts.push_back(k);
		laps.push_back(0);
		location.push_back(-1);
	}
}

HandleLap::~HandleLap()
{

}

void HandleLap::update()
{
	for (unsigned int i = 0, size = karts.size(); i < size; ++i)
	{
		if (location[i] != -1 && objectList->intersectLap(karts[i]->getCollisionBox()))
		{
			if (location[i] != 0)
			{
				++laps[i];
			}
			location[i] = -1;
		}
		else
		{
			int newLocation = objectList->getKartLocation(karts[i]->getCollisionBox());

			if (newLocation != -1 && location[i] == -1)
			{
				if (newLocation == objectList->getTrackLength() - 1)
				{
					--laps[i];
				}
			}

			if (newLocation != -1)
			{
				location[i] = newLocation;
			}
		}

		std::cout << i << ':' << laps[i] << std::endl;
	}
}

void HandleLap::registerKart(const Kart* kart)
{
	karts.push_back(kart);
	laps.push_back(0);
	location.push_back(-1);
}

void HandleLap::removeKart(const Kart* kart)
{
	for (std::vector<const Kart*>::const_iterator k = karts.begin(); k != karts.end(); ++k)
	{
		if (*k == kart)
		{
			karts.erase(k);
			break;
		}
	}
}

int HandleLap::getPlacing(const Kart* kart) const
{
	int index = 0;
	for (auto& k : karts)
	{
		if (kart == k)
		{
			break;
		}
		++index;
	}

	int placing = 1;
	for (int i = 0, size = static_cast<int>(karts.size()); i < size; ++i)
	{
		if (laps[i] > laps[index])
		{
			++placing;
			continue;
		}
		if (laps[i] == laps[index])
		{
			if (location[i] > location[index])
			{
				++placing;
			}
		}
	}

	return placing;
}