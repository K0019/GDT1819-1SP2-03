#include "CollisionChecker.h"

CollisionChecker::CollisionChecker()
{

}

bool CollisionChecker::collide(const Collider& lhs, const Collider& rhs)
{
	switch (lhs.getType())
	{
	case Collider::Type::AABB:
		switch (rhs.getType())
		{
		case Collider::Type::AABB:
			return collide(static_cast<const AABB*>(&lhs), static_cast<const AABB*>(&rhs));
		case Collider::Type::OBB:
			return collide(static_cast<const AABB*>(&lhs), static_cast<const OBB*>(&rhs));
		default:
			throw std::exception("Unimplemented collider type");
		}
		throw std::exception("Control should never reach here");
	case Collider::Type::OBB:
		switch (rhs.getType())
		{
		case Collider::Type::AABB:
			return collide(static_cast<const AABB*>(&rhs), static_cast<const OBB*>(&lhs));
		case Collider::Type::OBB:
			return collide(static_cast<const OBB*>(&lhs), static_cast<const OBB*>(&rhs));
		case Collider::Type::TRIANGLE:
			return collide(static_cast<const OBB*>(&lhs), static_cast<const Triangle*>(&rhs));
		default:
			throw std::exception("Unimplemented collider type");
		}
	case Collider::Type::TRIANGLE:
		switch (rhs.getType())
		{
		case Collider::Type::TRIANGLE:
			return collide(static_cast<const OBB*>(&rhs), static_cast<const Triangle*>(&lhs));
		default:
			throw std::exception("Unimplemented collider type");
		}
	default:
		throw std::exception("Unimplemented collider type");
	}
}

bool CollisionChecker::collide(const AABB* lhs, const AABB* rhs)
{
	return (((lhs->getCorner1().x >= rhs->getCorner1().x && lhs->getCorner1().x <= rhs->getCorner2().x) || (lhs->getCorner2().x >= rhs->getCorner1().x && lhs->getCorner2().x <= rhs->getCorner2().x)) &&
			((lhs->getCorner1().y >= rhs->getCorner1().y && lhs->getCorner1().y <= rhs->getCorner2().y) || (lhs->getCorner2().y >= rhs->getCorner1().y && lhs->getCorner2().y <= rhs->getCorner2().y)) &&
			((lhs->getCorner1().z >= rhs->getCorner1().z && lhs->getCorner1().z <= rhs->getCorner2().z) || (lhs->getCorner2().z >= rhs->getCorner1().z && lhs->getCorner2().z <= rhs->getCorner2().z)));
}

bool CollisionChecker::collide(const AABB* lhs, const OBB* rhs)
{
	OBB newAABB = OBB(lhs->getCorner1() + lhs->getCorner2() * 0.5, Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), fabs(lhs->getCorner2().x - lhs->getCorner1().x) * 0.5f, fabs(lhs->getCorner2().y - lhs->getCorner1().y) * 0.5f, fabs(lhs->getCorner2().z - lhs->getCorner1().z) * 0.5f);

	return collide(&newAABB, rhs);
}

bool CollisionChecker::collide(const OBB* lhs, const OBB* rhs)
{
	Vector3 distance = rhs->getPosition() - lhs->getPosition();
	return !( // 
		fabs(distance.Dot(lhs->getAxis(0))) > lhs->getHalf(0) + fabs(rhs->getHalf(0) * rhs->getAxis(0).Dot(lhs->getAxis(0))) + fabs(rhs->getHalf(1) * rhs->getAxis(1).Dot(lhs->getAxis(0))) + fabs(rhs->getHalf(2) * rhs->getAxis(2).Dot(lhs->getAxis(0))) // SA = AaxisX
		|| fabs(distance.Dot(lhs->getAxis(1))) > lhs->getHalf(1) + fabs(rhs->getHalf(0) * rhs->getAxis(0).Dot(lhs->getAxis(1))) + fabs(rhs->getHalf(1) * rhs->getAxis(1).Dot(lhs->getAxis(1))) + fabs(rhs->getHalf(2) * rhs->getAxis(2).Dot(lhs->getAxis(1))) // SA = AaxisY
		|| fabs(distance.Dot(lhs->getAxis(2))) > lhs->getHalf(2) + fabs(rhs->getHalf(0) * rhs->getAxis(0).Dot(lhs->getAxis(2))) + fabs(rhs->getHalf(1) * rhs->getAxis(1).Dot(lhs->getAxis(2))) + fabs(rhs->getHalf(2) * rhs->getAxis(2).Dot(lhs->getAxis(2))) // SA = AaxisZ
		|| fabs(distance.Dot(rhs->getAxis(0))) > fabs(lhs->getHalf(0) * lhs->getAxis(0).Dot(rhs->getAxis(0))) + fabs(lhs->getHalf(1) * lhs->getAxis(1).Dot(rhs->getAxis(0))) + fabs(lhs->getHalf(2) * lhs->getAxis(2).Dot(rhs->getAxis(0))) + rhs->getHalf(0) // SA = BaxisX
		|| fabs(distance.Dot(rhs->getAxis(1))) > fabs(lhs->getHalf(0) * lhs->getAxis(0).Dot(rhs->getAxis(1))) + fabs(lhs->getHalf(1) * lhs->getAxis(1).Dot(rhs->getAxis(1))) + fabs(lhs->getHalf(2) * lhs->getAxis(2).Dot(rhs->getAxis(1))) + rhs->getHalf(1) // SA = BaxisY
		|| fabs(distance.Dot(rhs->getAxis(2))) > fabs(lhs->getHalf(0) * lhs->getAxis(0).Dot(rhs->getAxis(2))) + fabs(lhs->getHalf(1) * lhs->getAxis(1).Dot(rhs->getAxis(2))) + fabs(lhs->getHalf(2) * lhs->getAxis(2).Dot(rhs->getAxis(2))) + rhs->getHalf(2) // SA = BaxisZ
		|| fabs(distance.Dot(lhs->getAxis(0).Cross(rhs->getAxis(0)))) > fabs(lhs->getHalf(1) * lhs->getAxis(2).Dot(rhs->getAxis(0))) + fabs(lhs->getHalf(2) * lhs->getAxis(1).Dot(rhs->getAxis(0))) + fabs(rhs->getHalf(1) * lhs->getAxis(0).Dot(rhs->getAxis(2))) + fabs(rhs->getHalf(2) * lhs->getAxis(0).Dot(rhs->getAxis(1))) // SP = AaxisX x BaxisX
		|| fabs(distance.Dot(lhs->getAxis(0).Cross(rhs->getAxis(1)))) > fabs(lhs->getHalf(1) * lhs->getAxis(2).Dot(rhs->getAxis(1))) + fabs(lhs->getHalf(2) * lhs->getAxis(1).Dot(rhs->getAxis(1))) + fabs(rhs->getHalf(0) * lhs->getAxis(0).Dot(rhs->getAxis(2))) + fabs(rhs->getHalf(2) * lhs->getAxis(0).Dot(rhs->getAxis(0))) // SP = AaxisX x BaxisY
		|| fabs(distance.Dot(lhs->getAxis(0).Cross(rhs->getAxis(2)))) > fabs(lhs->getHalf(1) * lhs->getAxis(2).Dot(rhs->getAxis(2))) + fabs(lhs->getHalf(2) * lhs->getAxis(1).Dot(rhs->getAxis(2))) + fabs(rhs->getHalf(0) * lhs->getAxis(0).Dot(rhs->getAxis(1))) + fabs(rhs->getHalf(1) * lhs->getAxis(0).Dot(rhs->getAxis(0))) // SP = AaxisX x BaxisZ
		|| fabs(distance.Dot(lhs->getAxis(1).Cross(rhs->getAxis(0)))) > fabs(lhs->getHalf(0) * lhs->getAxis(2).Dot(rhs->getAxis(0))) + fabs(lhs->getHalf(2) * lhs->getAxis(0).Dot(rhs->getAxis(0))) + fabs(rhs->getHalf(1) * lhs->getAxis(1).Dot(rhs->getAxis(2))) + fabs(rhs->getHalf(2) * lhs->getAxis(1).Dot(rhs->getAxis(1))) // SP = AaxisY x BaxisX
		|| fabs(distance.Dot(lhs->getAxis(1).Cross(rhs->getAxis(1)))) > fabs(lhs->getHalf(0) * lhs->getAxis(2).Dot(rhs->getAxis(1))) + fabs(lhs->getHalf(2) * lhs->getAxis(0).Dot(rhs->getAxis(1))) + fabs(rhs->getHalf(0) * lhs->getAxis(1).Dot(rhs->getAxis(2))) + fabs(rhs->getHalf(2) * lhs->getAxis(1).Dot(rhs->getAxis(0))) // SP = AaxisY x BaxisY
		|| fabs(distance.Dot(lhs->getAxis(1).Cross(rhs->getAxis(2)))) > fabs(lhs->getHalf(0) * lhs->getAxis(2).Dot(rhs->getAxis(2))) + fabs(lhs->getHalf(2) * lhs->getAxis(0).Dot(rhs->getAxis(2))) + fabs(rhs->getHalf(0) * lhs->getAxis(1).Dot(rhs->getAxis(1))) + fabs(rhs->getHalf(1) * lhs->getAxis(1).Dot(rhs->getAxis(0))) // SP = AaxisY x BaxisZ
		|| fabs(distance.Dot(lhs->getAxis(2).Cross(rhs->getAxis(0)))) > fabs(lhs->getHalf(0) * lhs->getAxis(1).Dot(rhs->getAxis(0))) + fabs(lhs->getHalf(1) * lhs->getAxis(0).Dot(rhs->getAxis(0))) + fabs(rhs->getHalf(1) * lhs->getAxis(2).Dot(rhs->getAxis(2))) + fabs(rhs->getHalf(2) * lhs->getAxis(2).Dot(rhs->getAxis(1))) // SP = AaxisZ x BaxisX
		|| fabs(distance.Dot(lhs->getAxis(2).Cross(rhs->getAxis(1)))) > fabs(lhs->getHalf(0) * lhs->getAxis(1).Dot(rhs->getAxis(1))) + fabs(lhs->getHalf(1) * lhs->getAxis(0).Dot(rhs->getAxis(1))) + fabs(rhs->getHalf(0) * lhs->getAxis(2).Dot(rhs->getAxis(2))) + fabs(rhs->getHalf(2) * lhs->getAxis(2).Dot(rhs->getAxis(0))) // SP = AaxisZ x BaxisY
		|| fabs(distance.Dot(lhs->getAxis(2).Cross(rhs->getAxis(2)))) > fabs(lhs->getHalf(0) * lhs->getAxis(1).Dot(rhs->getAxis(2))) + fabs(lhs->getHalf(1) * lhs->getAxis(0).Dot(rhs->getAxis(2))) + fabs(rhs->getHalf(0) * lhs->getAxis(2).Dot(rhs->getAxis(1))) + fabs(rhs->getHalf(1) * lhs->getAxis(2).Dot(rhs->getAxis(0))) // SP = AaxisZ x BaxisZ
		);
}

bool CollisionChecker::collide(const OBB* lhs, const Triangle* rhs)
{
	// OBB normals
	for (int i = 0; i < 3; ++i)
	{
		float min, max;
		min = max = (rhs->getV(0) - lhs->getPosition()).Dot(lhs->getAxis(i));
		for (int j = 1; j < 3; ++j)
		{
			float proj = (rhs->getV(j) - lhs->getPosition()).Dot(lhs->getAxis(i));
			if (proj < min)
			{
				min = proj;
			}
			else
			{
				max = proj;
			}
		}

		float r = lhs->getHalf(i);
		if (min > r || max < -r)
		{
			return false;
		}
	}

	// Triangle normal
	{
		Vector3 n = rhs->getNormal();
		if (fabs((rhs->getV(0) - lhs->getPosition()).Dot(n)) > fabs(lhs->getHalf(0) * lhs->getAxis(0).Dot(n)) + fabs(lhs->getHalf(1) * lhs->getAxis(1).Dot(n) + fabs(lhs->getHalf(2) * lhs->getAxis(2).Dot(n))))
		{
			return false;
		}
	}

	// For each edge pair
	{
		Vector3 v[3] = {
			rhs->getV(0) - lhs->getPosition(),
			rhs->getV(1) - lhs->getPosition(),
			rhs->getV(2) - lhs->getPosition()
		};
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				float min, max;
				Vector3 axis = lhs->getAxis(i).Cross(rhs->getF(j));
				if (axis.Length() < 0.00001f)
				{
					continue;
				}
				min = max = axis.Dot(v[0]);

				for (int k = 1; k < 3; ++k)
				{
					float proj = axis.Dot(v[k]);
					if (proj < min)
					{
						min = proj;
					}
					else
					{
						max = proj;
					}
				}

				float r = 0.0f;
				for (int k = 0; k < 3; ++k)
				{
					r += fabs(lhs->getHalf(k) * lhs->getAxis(k).Dot(axis));
				}

				if (min > r || max < -r)
				{
					return false;
				}
			}
		}
	}
	return true;
}