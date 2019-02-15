#pragma once
#ifndef MUITPLAYER_H
#define MUITPLAYER_H
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "Bear.h"
#include "Kart.h"
#include "UsefulFunctions.h"
class c_m_Player
{
public:
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos); // Function called when mouse moves to update camera

	c_m_Player();
	~c_m_Player();
	
	void update(GLFWwindow* window, double deltaTime, unsigned int uSpotLight); // Handle input
	void render() const; // Render bear if player is driving the kart

	bool hasKart() const; // Check if a kart is attached to the player

	void reset(); // Reset player position and orientation

	const Camera& getCam() const; // Return reference to camera object

private:
	static int Player_ID;
	int myPlayer_ID;

	Camera Cam;
	Kart* car;
};
#endif  
