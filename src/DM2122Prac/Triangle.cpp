#include "Triangle.h"

Triangle::Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2)
	: Collider(Collider::Type::TRIANGLE)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
}

Triangle::~Triangle()
{

}

const Vector3& Triangle::getV(int index) const
{
	return v[index];
}

const Vector3& Triangle::getF(int index) const
{
	return v[((index == 2) ? (0) : (index + 1))] - v[index];
}

Vector3 Triangle::getNormal() const
{
	return Vector3(v[1] - v[0]).Cross(Vector3(v[2] - v[0]));
}