#ifndef SCENE_H
#define SCENE_H
#include "GL/glew.h"
#include "GLFW/glfw3.h"

/* File: Scene.h
   Use: Base class of all Scene classes 
		Framework file, lightly modified */

class Scene
{
public:
	Scene() {} // Constructor
	virtual ~Scene() {} // Destructor

	virtual void Init() = 0; // Initialize
	virtual void Update(double dt, GLFWwindow* programID) = 0; // Update
	virtual void Render() = 0; // Render
	virtual void Exit() = 0; // Cleanup
};

#endif