#pragma once
#ifndef MUITPLAYER_H
#define MUITPLAYER_H
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "Bear.h"
#include "Kart.h"
#include "ModificationGateDetector.h"
#include "UsefulFunctions.h"
class c_m_Player
{
public:

	c_m_Player(unsigned int uSpotLight);
	~c_m_Player();
	
	void update(GLFWwindow* window, double deltaTime, unsigned int uSpotLight); // Handle input
	void updateCamera();

	void render(unsigned int uMatrixMVS) const;

	void reset(); // Reset player position and orientation

	const Kart* getCar() const;
	const Camera& getCam() const; // Return reference to camera object
private:
	static int Player_ID;
	int myPlayer_ID;

	Camera Cam;
	Kart* car;
	
};
#endif  
