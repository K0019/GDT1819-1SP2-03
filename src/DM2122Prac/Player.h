#ifndef PLAYER_H
#define PLAYER_H
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "Bear.h"
#include "Kart.h"
#include "UsefulFunctions.h"

/* File: Player.h
   Use: Class containing everything about the player, and handles input when not in kart */

class Player
{
public:
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos); // Function called when mouse moves to update camera

	Player(unsigned int uMatrixMVS, unsigned int uColorData); // Constructor
	~Player(); // Destructor

	void update(GLFWwindow* window, double deltaTime); // Handle input
	void updateCamera(GLFWwindow* window);

	void render() const; // Render bear if player is driving the kart

	void attachKart(Kart* kart); // Attach kart object to player
	bool hasKart() const; // Check if a kart is attached to the player

	void reset(); // Reset player position and orientation

	const Camera& getCam() const; // Return reference to camera object

protected:

private:
	static Camera camera; // Player camera

	Vector3 velocity; // Player velocity

	Bear* bear; // Bear object representing player, only rendered when kart is attached
	Kart* kart; // Pointer to kart acting as attachment point

	bool jumping, sprinting; // Store player states
};

#endif // !PLAYER_H
