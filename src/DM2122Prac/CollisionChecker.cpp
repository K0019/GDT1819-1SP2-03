#include "CollisionChecker.h"

CollisionChecker::CollisionChecker()
{

}

CollisionInfo CollisionChecker::collide(const Collider& lhs, const Collider& rhs)
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

CollisionInfo CollisionChecker::collide(const AABB* lhs, const AABB* rhs)
{
	return CollisionInfo(((lhs->getCorner1().x >= rhs->getCorner1().x && lhs->getCorner1().x <= rhs->getCorner2().x) || (lhs->getCorner2().x >= rhs->getCorner1().x && lhs->getCorner2().x <= rhs->getCorner2().x)) &&
			((lhs->getCorner1().y >= rhs->getCorner1().y && lhs->getCorner1().y <= rhs->getCorner2().y) || (lhs->getCorner2().y >= rhs->getCorner1().y && lhs->getCorner2().y <= rhs->getCorner2().y)) &&
			((lhs->getCorner1().z >= rhs->getCorner1().z && lhs->getCorner1().z <= rhs->getCorner2().z) || (lhs->getCorner2().z >= rhs->getCorner1().z && lhs->getCorner2().z <= rhs->getCorner2().z)));
}

CollisionInfo CollisionChecker::collide(const AABB* lhs, const OBB* rhs)
{
	OBB newAABB = OBB(lhs->getCorner1() + lhs->getCorner2(), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), fabs(lhs->getCorner2().x - lhs->getCorner1().x), fabs(lhs->getCorner2().y - lhs->getCorner1().y), fabs(lhs->getCorner2().z - lhs->getCorner1().z));

	return collide(&newAABB, rhs);
}

CollisionInfo CollisionChecker::collide(const OBB* lhs, const OBB* rhs)
{
	Vector3 distance = rhs->getPosition() - lhs->getPosition();
	return CollisionInfo(!( // 
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
		));
}

CollisionInfo CollisionChecker::collide(const OBB* lhs, const Triangle* rhs)
{
	Vector3 collisionNormal = rhs->getNormal();
	float distance = 10000000.0f;

	Vector3 v[3] = {
		rhs->getV(0) - lhs->getPosition(),
		rhs->getV(1) - lhs->getPosition(),
		rhs->getV(2) - lhs->getPosition()
	};
	// OBB normals
	for (int i = 0; i < 3; ++i)
	{
		float min, max;
		min = max = v[0].Dot(lhs->getAxis(i));
		for (int j = 1; j < 3; ++j)
		{
			float proj = v[j].Dot(lhs->getAxis(i));
			if (proj <= min)
			{
				min = proj;
			}
			else
			{
				max = proj;
			}
		}

		float r = lhs->getHalf(i);
		if (min >= r - 0.0f || max <= -r + 0.0f) // Not penetrating
		{
			return CollisionInfo(false);
		}
		//else // Penetrating
		//{
		//	if (r - min < r + max)
		//	{
		//		if (distance > r - min)
		//		{
		//			//collisionNormal = -lhs->getAxis(i);
		//			distance = r - min;
		//		}
		//	}
		//	else
		//	{
		//		if (distance > r + max)
		//		{
		//			//collisionNormal = lhs->getAxis(i);
		//			distance = r + max;
		//		}
		//	}
		//}
	}

	// Triangle normal
	{
		Vector3 n = rhs->getNormal();
		float thisDistance = fabs(lhs->getHalf(0) * lhs->getAxis(0).Dot(n) + lhs->getHalf(1) * lhs->getAxis(1).Dot(n) + lhs->getHalf(2) * lhs->getAxis(2).Dot(n)) - fabs(v[0].Dot(n));
		//if (fabs((rhs->getV(0) - lhs->getPosition()).Dot(n)) > fabs(lhs->getHalf(0) * lhs->getAxis(0).Dot(n)) + fabs(lhs->getHalf(1) * lhs->getAxis(1).Dot(n) + fabs(lhs->getHalf(2) * lhs->getAxis(2).Dot(n))))
		if (thisDistance <= 0.0f)
		{
			return CollisionInfo(false);
		}
		else
		{
			if (thisDistance < distance)
			{
				//collisionNormal = rhs->getNormal();
				distance = thisDistance;
			}
		}
	}

	// For each edge pair
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				float min, max;
				Vector3 axis = lhs->getAxis(i).Cross(rhs->getF(j));
				if (axis.Length() < 0.0001f)
				{
					continue;
				}
				min = max = axis.Dot(v[0]);

				for (int k = 1; k < 3; ++k)
				{
					float proj = axis.Dot(v[k]);
					if (proj <= min)
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

				if (min >= r - 0.0f || max <= -r + 0.0f)
				{
					return CollisionInfo(false);
				}
				//else
				//{
				//	if (r - min < r + max)
				//	{
				//		if (distance > r - min)
				//		{
				//			//collisionNormal = -axis;
				//			distance = r - min;
				//		}
				//	}
				//	else
				//	{
				//		if (distance > r + max)
				//		{
				//			//collisionNormal = axis;
				//			distance = r + max;
				//		}
				//	}
				//}
			}
		}
	}
	return CollisionInfo(true, collisionNormal, distance + 0.001f);
}