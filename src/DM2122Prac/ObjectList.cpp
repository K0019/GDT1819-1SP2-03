#include "ObjectList.h"

// Constructor
ObjectList::ObjectList()
{
	init();
}

// Destructor
ObjectList::~ObjectList()
{
	for (Object* object : objects)
	{
		delete object;
	}
	for (MeshPlaceable*& mesh : meshes)
	{
		delete mesh;
	}

	delete finishLineMesh;
}

// Load meshPlaceables
void ObjectList::init()
{
	meshes.push_back(MeshBuilder::GenerateMeshPlaceable("OBJ//PlaceableObjects//trackTest.obj", "Image//PlaceableObjects//trackTestUV.tga", type::SHADER_3, 10, 10, true));
	meshes.push_back(MeshBuilder::GenerateMeshPlaceable("OBJ//PlaceableObjects//trackTurnAngled.obj", "Image//PlaceableObjects//trackTestUV.tga", type::SHADER_3, 10, 10, true));
	meshes.push_back(MeshBuilder::GenerateMeshPlaceable("OBJ//PlaceableObjects//trackTurnCurved.obj", "Image//PlaceableObjects//trackTestUV.tga", type::SHADER_3, 10, 10, true));
	meshes.push_back(MeshBuilder::GenerateMeshPlaceable("OBJ//PlaceableObjects//modi_gate.obj", "Image//PlaceableObjects//modi_gate.tga", type::SHADER_3, 2, 10, false));

	finishLineMesh = MeshBuilder::GenerateMeshPlaceable("OBJ//PlaceableObjects//finishLine.obj", "Image//PlaceableObjects//finishLine.tga", type::SHADER_3, 2, 10, false);
	finishLine.push_back(new Object(finishLineMesh, 0, 0, Object::Rotation::NORTH));
	finishLine.push_back(new Object(meshes[0], 0, 0, Object::Rotation::NORTH));
}

// Attempt to add an object of meshPlaceable ID at a certain grid area and rotation
bool ObjectList::addObject(unsigned int ID, int gridX, int gridZ, Object::Rotation rotation)
{
	if (ID == 4) // Modification gate
	{
		if (!queryModificationGatePlacing(gridX, 0, gridZ, rotation))
			return false;
	}
	else
	{
		// Check if area is occupied
		if (queryOccupiedArea(ID, gridX, gridZ, rotation))
			return false;
	}
	
	// Add object
	objects.push_back((ID == 4 ? new ModificationGate(meshes[ID - 1], gridX, gridZ, rotation) : new Object(meshes[ID - 1], gridX, gridZ, rotation)));
	(*(objects.end() - 1))->setID(ID);
	return true;
}

void ObjectList::saveObject()
{ 
	ofstream file;
	file.open("save.txt");
	for (const auto& obj : objects)
	{
		file << obj->getID() << " ";
		file << obj->getGridX() << " ";
		file << obj->getGridY() << " ";
		file << obj->getGridZ() << " ";
		file << obj->getrotation() << endl;
	}
	file.close();
}

void ObjectList::deleteAll()
{
	ofstream file;
	file.open("delete.txt");
	for (const auto& obj : objects)
	{
		file << obj->getID() << " ";
		file << obj->getGridX() << " ";
		file << obj->getGridY() << " ";
		file << obj->getGridZ() << " ";
		file << obj->getrotation() << endl;
	}
	file.close();
}

// Attempt to delete any object that intersects a certain grid area and rotation
bool ObjectList::deleteObject(int gridX, int gridZ, Object::Rotation rotation)
{
	// Get any object that intersects with grid area
	std::vector<Object*>::iterator objectToDelete = queryOccupied(1, 1, 1, gridX, 1, gridZ, rotation);

	// If no object intersects, return false
	if (objectToDelete == objects.end())
		return false;
	else // If an object is found
	{
		// Delete object and return true
		delete *objectToDelete;
		objects.erase(objectToDelete);
		return true;
	}
}

// Render all objects
void ObjectList::renderObjects(unsigned int uMatrixMVS) const
{
	for (const Object* obj : objects)
	{
		obj->render(uMatrixMVS);
	}

	for (const Object* obj : finishLine)
	{
		obj->render(uMatrixMVS);
	}
}

// Render a single object, used for selection rendering
void ObjectList::renderSingleObject(unsigned int ID, unsigned int uMatrixMVS, bool valid, Object::Rotation rotation, MS& model) const
{
	model.PushMatrix();
	model.Rotate(rotation * 90.0f, 0.0f, 1.0f, 0.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	meshes[ID - 1]->Render(false, valid);
	model.PopMatrix();
}

// Retrieve an object that is at a certain grid area
std::vector<Object*>::iterator ObjectList::queryOccupied(int lengthX, int lengthY, int lengthZ, int gridX, int gridY, int gridZ, Object::Rotation rotation)
{
	AABB selection = Object::createAABB(lengthX, lengthY, lengthZ, gridX, gridY, gridZ, rotation);

	for (std::vector<Object*>::iterator obj = objects.begin(); obj != objects.end(); ++obj)
	{
		if (CollisionChecker::collide(selection, (*obj)->getAABB()) || CollisionChecker::collide((*obj)->getAABB(), selection))
			return obj;
	}

	return objects.end();
}

// Query if a meshPlaceable ID may be placed at a certain grid area and rotation
bool ObjectList::queryOccupiedArea(unsigned int ID, int gridX, int gridZ, Object::Rotation rotation) const
{
	return queryOccupiedArea(meshes[ID - 1]->getX(), 0, meshes[ID - 1]->getZ(), gridX, 0, gridZ, rotation);
}
// Retrieve an object that is at a certain grid area
bool ObjectList::queryOccupiedArea(int lengthX, int lengthY, int lengthZ, int gridX, int gridY, int gridZ, Object::Rotation rotation) const
{
	// Create AABB collider
	AABB selection = Object::createAABB(lengthX, lengthY, lengthZ, gridX, gridY, gridZ, rotation);
	
	// Check collision against each object
	for (std::vector<Object*>::const_iterator iter = objects.begin(); iter != objects.end(); ++iter)
	{
		if (CollisionChecker::collide(selection, (*iter)->getAABB()) || CollisionChecker::collide((*iter)->getAABB(), selection))
		{
			return true;
		}
	}

	for (std::vector<Object*>::const_iterator iter = finishLine.begin(); iter != finishLine.end(); ++iter)
	{
		if (CollisionChecker::collide(selection, (*iter)->getAABB()) || CollisionChecker::collide((*iter)->getAABB(), selection))
		{
			return true;
		}
	}
	// Return result
	return false;
}

bool ObjectList::queryModificationGatePlacing(int gridX, int gridY, int gridZ, Object::Rotation rotation) const
{
	// Create AABB collider
	AABB selection = Object::createAABB(2, 10, 10, gridX, gridY, gridZ, rotation);

	// Check collision against each object
	bool possible = false;

	for (const Object* obj : objects)
	{
		switch (obj->getID())
		{
		case 1:
			switch (rotation)
			{
			case Object::Rotation::NORTH:
			case Object::Rotation::SOUTH:
				if (obj->getrotation() == Object::Rotation::NORTH || obj->getrotation() == Object::Rotation::SOUTH)
				{
					if (obj->getGridX() >= gridX - 5 && obj->getGridX() < gridX + 5 && obj->getGridZ() == gridZ)
						possible = true;
					break;
				}
			case Object::Rotation::EAST:
			case Object::Rotation::WEST:
				if (obj->getrotation() == Object::Rotation::EAST || obj->getrotation() == Object::Rotation::WEST)
				{
					//if (obj->getGridX() == gridZ && obj->getGridZ() >= gridX - 5 && obj->getGridZ() < gridX + 5)
					if (obj->getGridX() == gridX && obj->getGridZ() >= gridZ - 5 && obj->getGridZ() < gridZ + 5)
						possible = true;
					break;
				}
			}
			break;
		case 4:
			if (CollisionChecker::collide(selection, obj->getAABB()) || CollisionChecker::collide(obj->getAABB(), selection))
				return false;
		}
	}

	return possible;
}

void ObjectList::searchTrackPath()
{
	raceTrack.clear();

	int gridX = 0, gridZ = 0;
	Object::Rotation rotation = Object::Rotation::NORTH;
	unsigned int ID = 1;

	while (true)
	{
		int offsetX1, offsetX2, offsetZ1, offsetZ2;
		switch (ID)
		{
		case 1:
			switch (rotation)
			{
			case Object::Rotation::NORTH:
				offsetX1 = gridX + 10;
				offsetX2 = gridX - 10;
				offsetZ1 = offsetZ2 = gridZ;
				break;
			case Object::Rotation::SOUTH:
				offsetX1 = gridX - 10;
				offsetX2 = gridX + 10;
				offsetZ1 = offsetZ2 = gridZ;
				break;
			case Object::Rotation::EAST:
				offsetX1 = offsetX2 = gridX;
				offsetZ1 = gridZ + 10;
				offsetZ2 = gridZ - 10;
				break;
			case Object::Rotation::WEST:
				offsetX1 = offsetX2 = gridX;
				offsetZ1 = gridZ - 10;
				offsetZ2 = gridZ + 10;
				break;
			}
			break;
		case 2:
		case 3:
			switch (rotation)
			{
			case Object::Rotation::NORTH:
				offsetX1 = gridX + 10;
				offsetZ1 = gridZ;
				offsetX2 = gridX;
				offsetZ2 = gridZ - 10;
				break;
			case Object::Rotation::SOUTH:
				offsetX1 = gridX;
				offsetZ1 = gridZ + 10;
				offsetX2 = gridX - 10;
				offsetZ2 = gridZ;
				break;
			case Object::Rotation::EAST:
				offsetX1 = gridX;
				offsetZ1 = gridZ - 10;
				offsetX2 = gridX - 10;
				offsetZ2 = gridZ;
				break;
			case Object::Rotation::WEST:
				offsetX1 = gridX + 10;
				offsetZ1 = gridZ;
				offsetX2 = gridX;
				offsetZ2 = gridZ + 10;
				break;
			}
			break;
		}

		if (raceTrack.size() > 1 && ((offsetX1 == 0 && offsetZ1 == 0) || (offsetX2 == 0 && offsetZ2 == 0)))
		{
			std::cout << "End" << std::endl;
			break;
		}

		std::vector<Object*>::iterator obj1 = queryOccupied(1, 1, 1, offsetX1, 5, offsetZ1, Object::Rotation::NORTH);
		std::vector<Object*>::iterator obj2 = queryOccupied(1, 1, 1, offsetX2, 5, offsetZ2, Object::Rotation::NORTH);
		for (const Object* obj : raceTrack)
		{
			if (obj1 != objects.end() && *obj1 == obj)
				obj1 = objects.end();
			if (obj2 != objects.end() && *obj2 == obj)
				obj2 = objects.end();
		}

		if (obj1 != objects.end() || obj2 != objects.end())
		{
			std::cout << "found" << std::endl;
			std::vector<Object*>::iterator objectToPick = (obj1 != objects.end() ? obj1 : obj2);

			raceTrack.push_back(*objectToPick);
			ID = (*objectToPick)->getID();
			rotation = (*objectToPick)->getrotation();
			if (objectToPick == obj1)
			{
				gridX = offsetX1;
				gridZ = offsetZ1;
			}
			else
			{
				gridX = offsetX2;
				gridZ = offsetZ2;
			}
		}
		else
		{
			std::cout << "not found" << std::endl;
			break;
		}
	}
}

bool ObjectList::intersectLap(const OBB& kartBox) const
{
	return CollisionChecker::collide(finishLine[0]->getAABB(), kartBox);
}

int ObjectList::getKartLocation(const OBB& kartBox) const
{
	int location = 0;

	for (const auto& obj : raceTrack)
	{
		if (CollisionChecker::collide(obj->getAABB(), kartBox))
		{
			return location;
		}
		++location;
	}

	return -1;
}

unsigned int ObjectList::getTrackLength() const
{
	return raceTrack.size();
}