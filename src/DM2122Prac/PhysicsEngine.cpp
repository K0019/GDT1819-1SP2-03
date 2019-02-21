#include "PhysicsEngine.h"

int PhysicsEngine::instances = 0;
PhysicsEngine Physics::physicsEngine = PhysicsEngine();

PhysicsEngine::PhysicsEngine()
{
	if (++instances > 1)
	{
		throw std::exception("Attempted to create more than one physics engine");
	}
}

PhysicsEngine::~PhysicsEngine()
{
	--instances;
}

void PhysicsEngine::registerMovingObject(MovingPhysicsObjectInternal* object)
{
	movingObjects.push_back(object);
}
void PhysicsEngine::registerStaticObject(StaticPhysicsObjectInternal* object)
{
	staticObjects.push_back(object);
}

void PhysicsEngine::removeMovingObject(MovingPhysicsObjectInternal* object)
{
	for (std::vector<MovingPhysicsObjectInternal*>::iterator iter = movingObjects.begin(); iter != movingObjects.end(); ++iter)
	{
		if (*iter == object)
		{
			movingObjects.erase(iter);
			break;
		}
	}
}
void PhysicsEngine::removeStaticObject(StaticPhysicsObjectInternal* object)
{
	for (std::vector<StaticPhysicsObjectInternal*>::iterator iter = staticObjects.begin(); iter != staticObjects.end(); ++iter)
	{
		if (*iter == object)
		{
			staticObjects.erase(iter);
			break;
		}
	}
}

void PhysicsEngine::update()
{
	updateHorizontal();
	/*for (auto& m : movingObjects)
	{
		m->moveObject(Vector3(0.0f, -0.5f, 0.0f));
	}
	updateVertical();*/

	//for (auto& m : movingObjects)
	//{
	//	for (auto& s : staticObjects)
	//	{
	//		CollisionInfo AABBtest = CollisionChecker::collide(m->getCollisionBox(), s->getAABB());
	//		if (AABBtest.isColliding())
	//		{
	//			std::vector<PackedCollision> collisions;
	//			
	//			int i = 0;

	//			//CollisionInfo largestDisplacement = CollisionInfo(false);
	//			for (std::vector<Triangle>::const_iterator iter = s->getTriangles().begin(); iter != s->getTriangles().end(); ++iter)
	//			{
	//				CollisionInfo triangleTest = CollisionChecker::collide(m->getCollisionBox(), *iter);
	//				if (triangleTest.isColliding())
	//				{
	//					std::cout << i << ": " << triangleTest.getDistance() << ' ' << triangleTest.getContactNormal().y << std::endl;
	//					collisions.push_back(PackedCollision(triangleTest, &m->getCollisionBox(), &(*iter)));

	//					/*if (largestDisplacement.isColliding())
	//					{
	//						if (triangleTest.getDistance() > largestDisplacement.getDistance())
	//						{
	//							largestDisplacement = triangleTest;
	//						}
	//					}
	//					else
	//					{
	//						largestDisplacement = triangleTest;
	//					}*/
	//				}
	//				++i;
	//			}

	//			std::sort(collisions.begin(), collisions.end(), PhysicsEngine::compareLowerDistanceWithY);
	//			
	//			std::cout << "Update" << std::endl;
	//			while (collisions.size())
	//			{
	//				Vector3 move = collisions[0].getInfo().getDistance() * collisions[0].getInfo().getContactNormal();
	//				std::cout << move.x << ' ' << move.y << ' ' << move.z << std::endl;
	//				m->moveObject(move);

	//				collisions.erase(collisions.begin());
	//				for (std::vector<PackedCollision>::iterator c = collisions.begin(); c != collisions.end(); )
	//				{
	//					c->recalculateCollision();
	//					if (c->getInfo().getDistance() <= 0.0f)
	//					{
	//						c = collisions.erase(c);
	//						continue;
	//					}

	//					++c;
	//				}
	//			}

	//			/*if (largestDisplacement.isColliding())
	//			{
	//				m->moveObject(largestDisplacement.getDistance() * largestDisplacement.getContactNormal());
	//			}*/
	//		}
	//	}
	//}
}

void PhysicsEngine::updateHorizontal()
{
	for (auto& m : movingObjects)
	{
		for (auto& s : staticObjects)
		{
			CollisionInfo AABBtest = CollisionChecker::collide(m->getCollisionBox(), s->getAABB());
			if (AABBtest.isColliding())
			{
				std::vector<PackedCollision> collisions;

				int i = 0;

				for (std::vector<Triangle>::const_iterator iter = s->getTriangles().begin(); iter != s->getTriangles().end(); ++iter)
				{
					CollisionInfo triangleTest = CollisionChecker::collide(m->getCollisionBox(), *iter);
					if (triangleTest.isColliding() && triangleTest.getContactNormal().y < 0.01f)
					{
						//std::cout << i << ": " << triangleTest.getDistance() << ' ' << triangleTest.getContactNormal().y << std::endl;
						collisions.push_back(PackedCollision(triangleTest, &m->getCollisionBox(), &(*iter)));
					}
					++i;
				}

				std::sort(collisions.begin(), collisions.end(), PhysicsEngine::compareLowerDistance);

				//std::cout << "Update horizontal" << std::endl;
				while (collisions.size())
				{
					Vector3 move = collisions[0].getInfo().getDistance() * collisions[0].getInfo().getContactNormal();
					//std::cout << move.x << ' ' << move.y << ' ' << move.z << std::endl;
					m->moveObject(move);

					collisions.erase(collisions.begin());
					for (std::vector<PackedCollision>::iterator c = collisions.begin(); c != collisions.end(); )
					{
						c->recalculateCollision();
						if (c->getInfo().getDistance() <= 0.0f)
						{
							c = collisions.erase(c);
							continue;
						}

						++c;
					}
				}
			}
		}
	}
}

void PhysicsEngine::updateVertical()
{
	for (auto& m : movingObjects)
	{
		for (auto& s : staticObjects)
		{
			CollisionInfo AABBtest = CollisionChecker::collide(m->getCollisionBox(), s->getAABB());
			if (AABBtest.isColliding())
			{
				std::vector<PackedCollision> collisions;

				int i = 0;

				for (std::vector<Triangle>::const_iterator iter = s->getTriangles().begin(); iter != s->getTriangles().end(); ++iter)
				{
					CollisionInfo triangleTest = CollisionChecker::collide(m->getCollisionBox(), *iter);
					if (triangleTest.isColliding() && triangleTest.getContactNormal().y > 0.01f)
					{
						//std::cout << i << ": " << triangleTest.getDistance() << ' ' << triangleTest.getContactNormal().y << std::endl;
						collisions.push_back(PackedCollision(triangleTest, &m->getCollisionBox(), &(*iter)));
					}
					++i;
				}

				std::sort(collisions.begin(), collisions.end(), PhysicsEngine::compareLowerDistance);

				//std::cout << "Update vertical" << std::endl;
				while (collisions.size())
				{
					Vector3 move = collisions[0].getInfo().getDistance() * collisions[0].getInfo().getContactNormal();
					//std::cout << move.x << ' ' << move.y << ' ' << move.z << std::endl;
					m->moveObject(move);

					collisions.erase(collisions.begin());
					for (std::vector<PackedCollision>::iterator c = collisions.begin(); c != collisions.end(); )
					{
						c->recalculateCollision();
						if (c->getInfo().getDistance() <= 0.0f)
						{
							c = collisions.erase(c);
							continue;
						}

						++c;
					}
				}
			}
		}
	}
}

bool PhysicsEngine::testCollision() const
{
	for (auto& m : movingObjects)
	{
		for (auto& s : staticObjects)
		{
			CollisionInfo AABBtest = CollisionChecker::collide(m->getCollisionBox(), s->getAABB());
			if (AABBtest.isColliding())
			{
				for (std::vector<Triangle>::const_iterator iter = s->getTriangles().begin(); iter != s->getTriangles().end(); ++iter)
				{
					CollisionInfo triangleTest = CollisionChecker::collide(m->getCollisionBox(), *iter);
					if (triangleTest.isColliding())
					{
						if (s->getPhysicsEnabled())
						{
							return true;
						}
						else
						{
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

bool PhysicsEngine::compareLowerDistance(const PackedCollision& info1, const PackedCollision& info2)
{
	return info1.getInfo().getDistance() > info2.getInfo().getDistance();
}
bool PhysicsEngine::compareLowerDistanceWithY(const PackedCollision& info1, const PackedCollision& info2)
{
	if (info1.getInfo().getContactNormal().y > 0.01f)
	{
		return false;
	}
	else if (info2.getInfo().getContactNormal().y > 0.01f)
	{
		return true;
	}
	else
	{
		return compareLowerDistance(info1, info2);
	}
}