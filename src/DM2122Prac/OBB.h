#ifndef OBB_H
#define OBB_H
#include "Collider.h"
#include "Vector3.h"

class OBB : public Collider
{
public:
	OBB(const Vector3& position, const Vector3& axisX, const Vector3& axisY, const Vector3& axisZ, float halfX, float halfY, float halfZ);
	~OBB();

	bool collide(const Collider& other) const override;

protected:

private:
	Vector3 position;
	Vector3 axisX, axisY, axisZ;
	float halfX, halfY, halfZ;
};

#endif // !OBB_H
