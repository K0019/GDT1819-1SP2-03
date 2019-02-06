#ifndef SCENE_1_H
#define SCENE_1_H
#include "Scene.h"
#include "ShaderPrograms.h"
#include "MeshBuilder.h"
#include "Bear.h"
#include "TreeWinter.h"
#include "MyMath.h"
#include "Mtx44.h"
#include "Camera.h"
#include "timer.h"
#include "Lamp.h"

/* File: Scene1.h
   Use: Holds the entire scene process together 
		Framework file, heavily modified */

class Scene1 : public Scene
{
public:
	Scene1(); // Constructor
	~Scene1(); // Destructor

	virtual void Init() override; // Initialize
	virtual void Update(double dt, GLFWwindow* programID) override; // Update the scene
	virtual void Render() override; // Render the scene
	virtual void Exit() override; // Cleanup

	Vector3 processInput(GLFWwindow* window); // Function called every update cycle, checks for keyboard input
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos); // Function called when mouse moves to update camera
	static void framebuffer_resize_callback(GLFWwindow* window, int width, int height); // Function called when window resizes to adapt render viewport

private:
	static unsigned int uMatrixMVS, uMatrixP, uColorData; // UBO handles for model, view, projection matrices, lighting and color, material data

	static unsigned m_programID; // Store OpenGL program ID, used for input
	static Camera camera; // Player camera
	static double deltaTime; // Store time since last frame, used for camera movement

	static Bear* bear; // The main model, the bear
	Base3DPoly* axes, * floor; // Axes and floor mesh
	TreeWinter* tree; // Tree mesh

	Lamp* lamp[1]; // Pointer to lamps

	const bool isPressed(GLFWwindow* window, GLenum key) const; // Checks if a key is pressed
};

#endif