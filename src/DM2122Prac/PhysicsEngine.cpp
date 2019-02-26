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
}

void PhysicsEngine::updateHorizontal()
{
	for (auto& m : movingObjects)
	{
		std::vector<PackedCollision> collisions;

		for (auto& s : staticObjects)
		{
			CollisionInfo AABBtest = CollisionChecker::collide(m->getCollisionBox(), s->getAABB());
			if (AABBtest.isColliding())
			{

				int i = 0;

				// Get collisions
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


				//std::sort(collisions.begin(), collisions.end(), PhysicsEngine::compareLowerDistance);

				////std::cout << "Update horizontal" << std::endl;
				//while (collisions.size())
				//{
				//	Vector3 move = collisions[0].getInfo().getDistance() * collisions[0].getInfo().getContactNormal();
				//	//std::cout << move.x << ' ' << move.y << ' ' << move.z << std::endl;
				//	m->moveObject(move);

				//	collisions.erase(collisions.begin());
				//	for (std::vector<PackedCollision>::iterator c = collisions.begin(); c != collisions.end(); )
				//	{
				//		c->recalculateCollision();
				//		if (c->getInfo().getDistance() <= 0.0f)
				//		{
				//			c = collisions.erase(c);
				//			continue;
				//		}

				//		++c;
				//	}
				//}
			}
		}

		// Remove collisions resulting in response that is in kart velocity direction
		for (std::vector<PackedCollision>::const_iterator collision = collisions.begin(); collision != collisions.end(); )
		{
			if (collision->getInfo().getContactNormal().Dot(m->getVelocity()) > 0.0f)
			{
				collision = collisions.erase(collision);
				continue;
			}
					
			++collision;
		}

		// Search for least collisions after moving and move accordingly, repeating until no collisions are left
		while (collisions.size())
		{
			std::vector<std::vector<CollisionInfo>> newCollisions;

			// Check new position against the detected triangles and get new collisions
			for (std::vector<PackedCollision>::const_iterator collision = collisions.begin(); collision != collisions.end(); ++collision)
			{
				newCollisions.push_back(std::vector<CollisionInfo>());
				OBB foreseePosition = m->getCollisionBox();
				foreseePosition.setPosition(m->getCollisionBox().getPosition() + collision->getInfo().getContactNormal() * collision->getInfo().getDistance());

				for (std::vector<PackedCollision>::const_iterator col = collisions.begin(); col != collisions.end(); ++col)
				{
					if (col == collision)
						continue;

					CollisionInfo newCollision = CollisionChecker::collide(foreseePosition, *(col->getRhs()));
					if (newCollision.isColliding())
						(newCollisions.end() - 1)->push_back(newCollision);
				}
			}

			// Get lowest number of collisions' case
			std::vector<std::vector<CollisionInfo>>::iterator lowestCollision = newCollisions.begin();
			for (std::vector<std::vector<CollisionInfo>>::iterator iter = newCollisions.begin(); iter != newCollisions.end(); ++iter)
			{
				if (iter->size() < lowestCollision->size())
					lowestCollision = iter;
			}

			// Get index within respective collisions vector
			int index = 0;
			for (std::vector<std::vector<CollisionInfo>>::iterator iter = newCollisions.begin(); iter != newCollisions.end(); ++iter, ++index)
			{
				if (iter == lowestCollision)
					break;
			}

			// Move kart
			m->moveObject(collisions[index].getInfo().getContactNormal() * collisions[index].getInfo().getDistance());
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

void PhysicsEngine::empty()
{
	staticObjects.clear();
}