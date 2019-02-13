#ifndef OBB_H
#define OBB_H
#include "Collider.h"
#include "Mtx44.h"
#include "Vector3.h"

class OBB : public Collider
{
public:
	OBB(const Vector3& position, const Vector3& axisX, const Vector3& axisY, const Vector3& axisZ, float halfX, float halfY, float halfZ);
	~OBB();

	void rotate(float degree, const Vector3& axis);
	void setRotation(const Vector3& axisX, const Vector3& axisY, const Vector3& axisZ);
	void setPosition(const Vector3& position);

	const Vector3& getPosition() const;
	const Vector3& getAxis(int index) const;
	float getHalf(int index) const;
protected:

private:
	Vector3 position;
	Vector3 axis[3]; // x, y, z
	float half[3]; // x, y, z
};

#endif // !OBB_H
