#ifndef PLACEOBJECTHANDLER_H
#define PLACEOBJECTHANDLER_H
#include "ObjectList.h"
#include "Player.h"
#include "Hotbar.h"
#include "UsefulFunctions.h"

/* File: PlaceObjectHandler.h
   Use: Handle input for object placing */

class PlaceObjectHandler
{
public:
	PlaceObjectHandler(ObjectList* objectList, const Player* player, const Hotbar* hotbar); // Constructor
	~PlaceObjectHandler(); // Destructor

	void update(GLFWwindow* window, double dt); // Handle object placing/deleting input

	void drawSelection(unsigned int uMatrixMVS) const; // Draw selection

	Object::Rotation getRotation() const; // Get selection rotation

	void  Loadmap();
protected:

private:
	ObjectList* objectList; // Get ObjectList instance within scene
	const Player* player; // Get Player instance within scene
	const Hotbar* hotbar; // Get Hotbar instance within scene

	bool leftClicked, rightClicked; // Store whether mouse buttons were clicked last frame

	double rotateBounceTime; // Bounce time for rotation input
	Object::Rotation rotation; // Current selection rotation
	int gridx,gridy, gridz, rotate;
	unsigned int id;
};

#endif // !PLACEOBJECTHANDLER_H
