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

}

bool PhysicsEngine::testCollision() const
{
	for (auto& m : movingObjects)
	{
		for (auto& s : staticObjects)
		{
			/*if (CollisionChecker::collide(m->getCollisionBox(), s->getAABB()))
			{*/
				for (std::vector<Triangle>::const_iterator iter = s->getTriangles().begin(); iter != s->getTriangles().end(); ++iter)
				{
					if (CollisionChecker::collide(m->getCollisionBox(), *iter))
					{
						return true;
					}
				}
			//}
		}
	}

	return false;
}