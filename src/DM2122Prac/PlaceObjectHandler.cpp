#include "PlaceObjectHandler.h"

// Constructor
PlaceObjectHandler::PlaceObjectHandler(ObjectList* objectList, const Player* player, const Hotbar* hotbar)
	: leftClicked(false)
	, rotation(Object::Rotation::NORTH)
	, rotateBounceTime(0.0)
	, objectList(objectList)
	, player(player)
	, hotbar(hotbar)
	, gridy(0)
{
}

// Destructor
PlaceObjectHandler::~PlaceObjectHandler()
{

}

// Handle object placing/deleting input
void PlaceObjectHandler::update(GLFWwindow* window, double dt)
{
	//// clear world function
	//if (isPressed(window, GLFW_KEY_B))
	//{
	//	objectList->deleteAll();
	//}
	//// Save function
	//if (isPressed(window, GLFW_KEY_N))
	//{
	//	objectList->saveObject();
	//}
	//// Load save file
	//if (isPressed(window, GLFW_KEY_M))
	//{
	//	Loadmap();
	//}
	// Rotate selection
	if (isPressed(window, GLFW_KEY_R))
	{
		if (rotateBounceTime <= 0.0)
		{
			++rotation;
			rotateBounceTime = 0.3;
		}
		else
		{
			rotateBounceTime -= dt;
		}
	}
	else
	{
		rotateBounceTime = 0.0;
	}

	// Place object
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1))
	{
		if (!leftClicked && hotbar->querySelection() != 0)
		{
			Vector3 intersection;
			// Get point of intersection
			if (linePlaneIntersect(player->getCam().pos, player->getCam().front, Vector3(), Vector3(0.0f, 1.0f, 0.0f), intersection))
			{
				// Convert intersection point into grid coordinates
				int gridX = (static_cast<int>(intersection.x) >> 1) - (intersection.x < 0.0f);
				int gridY = (static_cast<int>(intersection.z) >> 1) - (intersection.z < 0.0f);
				
				// Attempt to add an object
				if (objectList->addObject(hotbar->querySelection(), gridX, gridY, rotation))
					leftClicked = true;
			}
		}
	}
	else
	{
		leftClicked = false;
	}

	// Delete object
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
	{
		if (!rightClicked)
		{
			Vector3 intersection;
			// Get intersection point
			if (linePlaneIntersect(player->getCam().pos, player->getCam().front, Vector3(), Vector3(0.0f, 1.0f, 0.0f), intersection))
			{
				// Convert intersection point into grid cordinates
				int gridX = (static_cast<int>(intersection.x) >> 1) - (intersection.x < 0.0f);
				int gridZ = (static_cast<int>(intersection.z) >> 1) - (intersection.z < 0.0f);

				// Attempt to delete an object
				if (objectList->deleteObject(gridX, gridZ, Object::Rotation::NORTH))
					rightClicked = true;
			}
		}
	}
	else
	{
		rightClicked = false;
	}
}

// Get selection rotation
Object::Rotation PlaceObjectHandler::getRotation() const
{
	return rotation;
}

void PlaceObjectHandler::Loadmap()
{
	objectList->deleteAll();
	ifstream read;
	read.open("save.txt");
	//for (int i = 0; i < 100; i++)
	while (read >> id >> gridx >> gridy >> gridz >> rotate)
	{
		//read >> id >> gridx >> gridy >> gridz >> rotate;
		if (id == 0 && gridx == 0 && gridz == 0 && rotate == 0)
		{
		}
		else
		{
			objectList->addObject(id, gridx, gridz, static_cast<Object::Rotation>(rotate));
		}

	}
	read.close();

	objectList->searchTrackPath();
}

// Draw selection
void PlaceObjectHandler::drawSelection(unsigned int uMatrixMVS) const
{
	Vector3 intersection;
	// Get camera intersection with floor plane
	if (hotbar->querySelection() != 0 && linePlaneIntersect(player->getCam().pos, player->getCam().front, Vector3(), Vector3(0.0f, 1.0f, 0.0f), intersection))
	{
		// Convert intersection point into grid coordinates
		int gridX = (static_cast<int>(intersection.x) >> 1) - (intersection.x < 0.0f);
		int gridZ = (static_cast<int>(intersection.z) >> 1) - (intersection.z < 0.0f);

		// Convert grid coordinates into opengl coordinates
		int x = static_cast<int>(((intersection.x + 5.0f) / 10.0f) - ((intersection.x + 5.0f < 0.0f) ? (1.0f) : (0.0f)));
		int z = static_cast<int>(((intersection.z + 5.0f) / 10.0f) - ((intersection.z + 5.0f < 0.0f) ? (1.0f) : (0.0f)));

		// Setup translation
		MS model;
		model.LoadIdentity();
		model.PushMatrix();
		model.Translate(gridX * 2.0f, 0.0f, gridZ * 2.0f);

		// Render the object
		if (hotbar->querySelection() == 4)
		{
			objectList->renderSingleObject(hotbar->querySelection(), uMatrixMVS, objectList->queryModificationGatePlacing(gridX, 0, gridZ, getRotation()), getRotation(), model);
		}
		else
		{
			objectList->renderSingleObject(hotbar->querySelection(), uMatrixMVS, !objectList->queryOccupiedArea(hotbar->querySelection(), gridX, gridZ, getRotation()), getRotation(), model);
		}
		model.PopMatrix();
	}
}