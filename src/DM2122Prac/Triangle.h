#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Collider.h"
#include "Mtx44.h"
#include "Vector3.h"

class Triangle : public Collider
{
public:
	Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2);
	~Triangle();

	void transform(const Mtx44& transformationMatrix);

	const Vector3& getV(int index) const;
	const Vector3& getF(int index) const;
	Vector3 getNormal() const;
protected:

private:
	Vector3 v[3];
};

#endif // !TRIANGLE_H
