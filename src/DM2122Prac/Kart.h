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
#include "Definitions.h"
#include "UsefulFunctions.h"

/* File: Kart.h
   Use: Class containing information about the kart
		Handles kart rendering, and movement when this is attached to the player */

class Kart
{
public:
	Kart(Mesh* body, Mesh* wheel, Mesh* steeringWheel, 
		const Vector3& wheelFrontLeftPos, const Vector3& wheelFrontRightPos, 
		const Vector3& wheelBackLeftPos, const Vector3& wheelBackRightPos,
		const Vector3& steeringWheelPos, unsigned int uSpotLight); // Constructor
	~Kart(); // Destructor

	void update(GLFWwindow* window, double deltaTime, unsigned int uSpotLight); // Move the kart and handle input
	void render(unsigned int uMatrixMVS) const; // Render the kart

	void reset(); // Reset kart position, rotation, velocity and gear

	const Vector3& getPos() const; // Get kart position in world space
	const Vector3& getVel() const; // Get kart velocity
	double getYaw() const; // Get kart rotation
	Vector3 getCamDir() const;
	std::string getSpeedText() const; // Generate HUD text for speed
	std::string getGear() const; // Generate HUD text for drive gear

	void stop(); // Set kart velocity to 0
protected:

private:

	Vector3 UP, FRONT; // front and  up vector 
	Vector3 pos, velocity; // Position and velocity of kart
	Vector3 velocitydir, camdir; // Direction of the velocity
	// Front vector is always  rotated by up vector for yaw  
	// Up vecotr is always rotated by  Front vector for row 
	// For pitch  front and up vector is rotated  by front X Up;
	double yaw, pitch, roll, speed, turnForce, wheelRotation; // Misc. variables (UNUSED - pitch & roll)
	double turnDegree;
	Mesh* body, * wheel, * steeringWheel; // Meshes loaded for kart
	Vector3 frontLeftPos, frontRightPos, backLeftPos, backRightPos, steeringPos; // Position of the wheel and steering wheel relative to kart position
	bool drifton;
	SpotLight spotLights[2]; // Spotlight structures to render headlight effect
	void GetYawPitchRoll(Vector3 forward, Vector3 up, double& yaw, double& pitch, double& roll);
	bool isDriveGear; // Drive/Reverse gear
	double gearShiftDelay; // Bounce time for changing between drive and reverse gear
};

#endif // !KART_H
