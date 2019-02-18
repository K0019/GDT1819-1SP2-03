#ifndef KART_H
#define KART_H
#include <string>
#include <sstream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Vector3.h"
#include "SpotLight.h"
#include "MatrixStack.h"
#include "Source/Mesh.h"
#include "CollisionChecker.h"
#include "MovingPhysicsObject.h"
#include "Definitions.h"
#include "UsefulFunctions.h"

/* File: Kart.h
   Use: Class containing information about the kart
		Handles kart rendering, and movement when this is attached to the player */

class Kart : public MovingPhysicsObject
{
public:
	Kart(Mesh* body, Mesh* wheel, Mesh* steeringWheel, 
		const Vector3& wheelFrontLeftPos, const Vector3& wheelFrontRightPos, 
		const Vector3& wheelBackLeftPos, const Vector3& wheelBackRightPos,
		const Vector3& steeringWheelPos, unsigned int uSpotLight, const OBB& obb); // Constructor
	~Kart(); // Destructor

	void update(GLFWwindow* window, double deltaTime, unsigned int uSpotLight); // Move the kart and handle input
	void update(GLFWwindow* window, double deltaTime, unsigned int uSpotLight,int PlayerID); // Move the kart and handle input
	void render(unsigned int uMatrixMVS) const; // Render the kart

	void reset(); // Reset kart position, rotation, velocity and gear

	const Vector3& getPos() const; // Get kart position in world space
	const Vector3& getVel() const; // Get kart velocity
	double getYaw() const; // Get kart rotation

	std::string getSpeedText() const; // Generate HUD text for speed
	std::string getGear() const; // Generate HUD text for drive gear

	void stop(); // Set kart velocity to 0
protected:

private:

	Vector3 pos, velocity; // Position and velocity of kart
	double yaw, pitch, roll, speed, turnForce, wheelRotation; // Misc. variables (UNUSED - pitch & roll)
	Mesh* body, * wheel, * steeringWheel; // Meshes loaded for kart
	Vector3 frontLeftPos, frontRightPos, backLeftPos, backRightPos, steeringPos; // Position of the wheel and steering wheel relative to kart position

	SpotLight spotLights[2]; // Spotlight structures to render headlight effect

	bool isDriveGear; // Drive/Reverse gear
	double gearShiftDelay; // Bounce time for changing between drive and reverse gear
};

#endif // !KART_H
