#include "OBB.h"

OBB::OBB(const Vector3& position, const Vector3& axisX, const Vector3& axisY, const Vector3& axisZ, float halfX, float halfY, float halfZ)
	: Collider(Collider::Type::OBB)
	, position(position)
{
	axis[0] = axisX;
	axis[1] = axisY;
	axis[2] = axisZ;
	half[0] = halfX;
	half[1] = halfY;
	half[2] = halfZ;
}

OBB::~OBB()
{

}

void OBB::rotate(float degree, const Vector3& axis)
{
	Mtx44 rotate;
	rotate.SetToRotation(degree, axis.x, axis.y, axis.z);

	for (int i = 0; i < 3; ++i)
	{
		this->axis[i] = rotate * this->axis[i];
	}
}

void OBB::setRotation(const Vector3& axisX, const Vector3& axisY, const Vector3& axisZ)
{
	axis[0] = axisX;
	axis[1] = axisY;
	axis[2] = axisZ;
}

void OBB::setPosition(const Vector3& position)
{
	this->position = position;
}

const Vector3& OBB::getPosition() const
{
	return position;
}

const Vector3& OBB::getAxis(int index) const
{
	return axis[index];
}

float OBB::getHalf(int index) const
{
	return half[index];
}