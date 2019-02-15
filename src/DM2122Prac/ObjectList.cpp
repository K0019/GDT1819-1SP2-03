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
}

// Load meshPlaceables
void ObjectList::init()
{
	meshes.push_back(MeshBuilder::GenerateMeshPlaceable("OBJ//PlaceableObjects//trackTest.obj", "Image//PlaceableObjects//trackTestUV.tga", type::SHADER_3, 10, 10, true));
	meshes.push_back(MeshBuilder::GenerateMeshPlaceable("OBJ//PlaceableObjects//trackTurnAngled.obj", "Image//PlaceableObjects//trackTestUV.tga", type::SHADER_3, 10, 10, true));
	meshes.push_back(MeshBuilder::GenerateMeshPlaceable("OBJ//PlaceableObjects//trackTurnCurved.obj", "Image//PlaceableObjects//trackTestUV.tga", type::SHADER_3, 10, 10, true));
	/*
		ADD MODIFICATION GATE HERE
	*/
	meshes.push_back(MeshBuilder::GenerateMeshPlaceable("OBJ//PlaceableObjects//trackTurnCurved.obj", "Image//PlaceableObjects//stone.tga", type::SHADER_3, 10, 2, false));
	// Source: https://www.turbosquid.com/3d-models/free-3ds-model-tree-arbaro-cheetah/669717
	meshes.push_back(MeshBuilder::GenerateMeshPlaceable("OBJ//PlaceableObjects//deadTree.obj", "Image//PlaceableObjects//deadTree.tga", type::SHADER_3, 2, 2, true));
	// Source: https://www.turbosquid.com/3d-models/free-obj-model-mapped/723445
	meshes.push_back(MeshBuilder::GenerateMeshPlaceable("OBJ//PlaceableObjects//stone.obj", "Image//PlaceableObjects//stone.tga", type::SHADER_3, 2, 2, true));
	meshes.push_back(MeshBuilder::GenerateMeshPlaceable("OBJ//PlaceableObjects//woodenBox.obj", "Image//PlaceableObjects//woodenBox.tga", type::SHADER_3, 2, 2, true));
	// Source: https://www.virtual-lands-3d.com/trash-bin.html
	meshes.push_back(MeshBuilder::GenerateMeshPlaceable("OBJ//PlaceableObjects//trashCan.obj", "Image//PlaceableObjects//trashCan.tga", type::SHADER_3, 1, 1, true));
	// Source: https://www.turbosquid.com/3d-models/free-bucket-3d-model/629635
	meshes.push_back(MeshBuilder::GenerateMeshPlaceable("OBJ//PlaceableObjects//fireBucket.obj", "Image//PlaceableObjects//fireBucket.tga", type::SHADER_3, 2, 2, true));
	
}

// Attempt to add an object of meshPlaceable ID at a certain grid area and rotation
bool ObjectList::addObject(unsigned int ID, int gridX, int gridZ, Object::Rotation rotation)
{
	// Check if area is occupied
	if (queryOccupiedArea(ID, gridX, gridZ, rotation))
		return false;
	
	// Add object
	objects.push_back(new Object(meshes[ID - 1], gridX, gridZ, rotation));
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
		if (CollisionChecker::collide(selection, (*obj)->getAABB()))
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

	bool collides = false;
	
	// Check collision against each object
	for (const Object* obj : objects)
	{
		if (CollisionChecker::collide(selection, obj->getAABB()) || CollisionChecker::collide(obj->getAABB(), selection))
		{
			collides = true;
			break;
		}
	}
	// Return result
	return collides;
}