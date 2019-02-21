#ifndef HANDLELAP_H
#define HANDLELAP_H
#include "Kart.h"
#include "ObjectList.h"

class HandleLap
{
public:
	HandleLap(const ObjectList* objectList);
	explicit HandleLap(const ObjectList* objectList, std::initializer_list<const Kart*> karts);
	~HandleLap();

	void update();

	void registerKart(const Kart* kart);
	void removeKart(const Kart* kart);

	int getPlacing(const Kart* kart) const;
protected:

private:
	const ObjectList* objectList;

	std::vector<const Kart*> karts;
	std::vector<int> laps;
	std::vector<int> location;
};

#endif // !HANDLELAP_H
