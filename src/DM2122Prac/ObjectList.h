#ifndef OBJECTLIST_H
#define OBJECTLIST_H
#include <vector>
#include <fstream>
#include "Object.h"
#include "Source/MeshBuilder.h"

using namespace std;
/* File: ObjectList.h
   Use: Contain all objects placed and all meshPlaceables */

class ObjectList
{
public:
	ObjectList(); // Constructor
	~ObjectList(); // Destructor
	
	bool addObject(unsigned int ID, int gridX, int gridZ, Object::Rotation rotation); // Attempt to add an object of meshPlaceable ID at a certain grid area and rotation
	void saveObject();
	bool loadObject(unsigned int ID, int gridX, int gridZ, Object::Rotation rotation);
	void deleteAll();
	bool deleteObject(int gridX, int gridZ, Object::Rotation rotation); // Attempt to delete any object that intersects a certain grid area and rotation
	void renderObjects(unsigned int uMatrixMVS) const; // Render all objects
	void renderSingleObject(unsigned int ID, unsigned int uMatrixMVS, bool valid, Object::Rotation rotation, MS& model) const; // Render a single object, used for selection rendering

	bool queryOccupiedArea(unsigned int ID, int gridX, int gridZ, Object::Rotation rotation) const; // Query if a meshPlaceable ID may be placed at a certain grid area and rotation
	bool queryOccupiedArea(int lengthX, int lengthY, int lengthZ, int gridX, int gridY, int gridZ, Object::Rotation rotation) const; // Query if a custom area intersects with any placed object
	std::vector<Object>::iterator queryOccupied(int lengthX, int lengthY, int lengthZ, int gridX, int gridY, int gridZ, Object::Rotation rotation); // Retrieve an object that is at a certain grid area
protected:

private:
	void init(); // Load meshPlaceables
	std::vector<Object> objects; // Container of all objects placed
	std::vector<MeshPlaceable*> meshes; // Container of all meshPlaceables
};

#endif // !OBJECTLIST_H
