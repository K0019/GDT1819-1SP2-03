#pragma once
#ifndef SCENE_GAME_H
#define SCENE_GAME_H
#include <sstream>
#include "../DM2122Prac/Source/Scene.h"
#include "source/ShaderPrograms.h"
#include "source/MeshBuilder.h"
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

#include "PlaceObjectHandler.h"

class SceneGame : public Scene
{
public:
	SceneGame();
	virtual ~SceneGame();

	virtual void Init() override; // Initialize
	virtual void Update(double dt, GLFWwindow* programID) override; // Update the scene
	virtual void Render() override; // Render the scene
	virtual void Exit() override; // Cleanup

	//this is for rendering 
	void renderView(c_m_Player* player);
	
	// this is for updating 
	void UpdateView(c_m_Player* player);

	void processInput(GLFWwindow* window); // Function called every update cycle, checks for keyboard input
	static void framebuffer_resize_callback(GLFWwindow* window, int width, int height); // Function called when window resizes to adapt render viewport

private :
	static unsigned int uMatrixMVS, uMatrixP, uColorData, uSpotLight; // UBO handles for model, view, projection matrices, lighting and color, material data

	static unsigned m_programID; // Store OpenGL program ID, used for input
	static double deltaTime; // Store time since last frame, used for camera movement
	static int width, height;
	static double enterBounce;

	Base3DPoly* axes, *skybox; // Axes and skybox mesh
	MeshFloor* floor; // Floor mesh
	c_m_Player* player[2]; // Player object

	Kart* kart; // Kart object

	Lamp* lamp[1]; // Pointer to lamp

	MeshText* text; // Text mesh

	bool isCullFace; // Cull face setting

	std::string calculateFPS() const; // Generate HUD text for FPS
	//void drawCoordinates() const; // Render coordinate text to screen
};

#endif