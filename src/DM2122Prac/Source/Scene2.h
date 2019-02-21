#ifndef SCENE_2_H
#define SCENE_2_H
#include <fstream>
#include <sstream>
#include "Scene.h"
#include "ShaderPrograms.h"
#include "MeshBuilder.h"
#include "MyMath.h"
#include "Mtx44.h"
#include "timer.h"
#include "Lamp.h"
#include "Player.h"
#include "Kart.h"
#include "MeshFloor.h"
#include "ObjectList.h"
#include "Hotbar.h"
#include "CollisionChecker.h"
#include "UsefulFunctions.h"
#include "c_m_Player.h"
#include "HandleLap.h"

#include "PlaceObjectHandler.h"

using namespace std;
/* File: Scene2.h
   Use: Holds the entire scene process together
		Framework file, heavily modified */

class Scene2 : public Scene
{
public:
	Scene2(); // Constructor
	virtual ~Scene2(); // Destructor

	virtual void Init() override; // Initialize
	virtual void Update(double dt, GLFWwindow* programID) override; // Update the scene
	virtual void Render() override; // Render the scene
	virtual void Exit() override; // Cleanup

	void processInput(GLFWwindow* window); // Function called every update cycle, checks for keyboard input
	static void framebuffer_resize_callback(GLFWwindow* window, int width, int height); // Function called when window resizes to adapt render viewport
	
private:
	static unsigned int uMatrixMVS, uMatrixP, uColorData, uSpotLight; // UBO handles for model, view, projection matrices, lighting and color, material data

	static unsigned m_programID; // Store OpenGL program ID, used for input
	static double deltaTime; // Store time since last frame, used for camera movement
	static int width, height;
	static double enterBounce;

	Base3DPoly* axes, * skybox; // Axes and skybox mesh
	MeshFloor* floor; // Floor mesh
	Hotbar* hotbar; // Hotbar object
	ObjectList objectList; // List of objects (meshPlaceable)
	Player* player; // Player object

	PlaceObjectHandler* placeObjHandler; // Handler of placing of objects
	HandleLap* handleLap;

	//////////////
	Kart* kart; // Kart object
	///////////////
	Lamp* lamp[1]; // Pointer to lamp

	MeshText* text; // Text mesh

	bool isCullFace; // Cull face setting

	std::string calculateFPS() const; // Generate HUD text for FPS
	void drawCoordinates() const; // Render coordinate text to screen
};

#endif