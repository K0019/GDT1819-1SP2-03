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
	Kart(Mesh* basic, Mesh* pikachu, Mesh* eevee, Mesh*mew, Mesh*squirtle,
         Mesh* basic_wheel, Mesh* pikachu_wheel, Mesh* eevee_wheel, Mesh* mew_wheel, Mesh* squirtle_wheel,Mesh* steeringWheel,
		const Vector3& wheelFrontLeftPos, const Vector3& wheelFrontRightPos, 
		const Vector3& wheelBackLeftPos, const Vector3& wheelBackRightPos,
		const Vector3& steeringWheelPos, unsigned int uSpotLight, const OBB& obb); // Constructor
	~Kart(); // Destructor

	void update(GLFWwindow* window, double deltaTime); // Move the kart and handle input
	void updateOpenGL(unsigned int uSpotLight);
	void update(GLFWwindow* window, double deltaTime, unsigned int uSpotLight, int PlayerID); // Move the kart and handle input
	void render(unsigned int uMatrixMVS) const; // Render the kart

	void reset(); // Reset kart position, rotation, velocity and gear

	const Vector3& getPos() const; // Get kart position in world space
	const Vector3& getVel() const; // Get kart velocity
	double getYaw() const; // Get kart rotation

	std::string getSpeedText() const; // Generate HUD text for speed
	std::string getGear() const; // Generate HUD text for drive gear
	
	enum status
	{
		e_basic,
		e_pikachu,
		e_eevee,
		e_mew,
		e_squirtle,
	};
	enum buff
	{
		b_stun,
		b_slow,
		b_confuse,
		b_speed,
		b_nothing,
	};
	bool player_used = false;
	bool player_eevee_up = false;

	static bool player2_slow;
	static bool player1_slow;
	static bool player2_invert_control;
	static bool player1_invert_control;
	static bool player1_stun;
	static bool player2_stun;

	void changeStatus();

	bool getInvert2();
	bool getInvert1();
	bool getSlow2();
	bool getSlow1();
	bool getStun1();
	bool getStun2();

	


	status getStatus() const;
	bool get_used() const;
	buff getBuff() const;

	void stop(); // Set kart velocity to 0
protected:
	virtual void moveObject(const Vector3& displacement);

private:
	buff m_buff;
	status m_status;
	Vector3 pos, velocity; // Position and velocity of kart
	double yaw, pitch, roll, speed, turnForce, wheelRotation; // Misc. variables (UNUSED - pitch & roll)
	Mesh* basic, * pikachu, * eevee , * mew , * squirtle , * basic_wheel, *pikachu_wheel, *eevee_wheel, *mew_wheel, *squirtle_wheel, * steeringWheel; // Meshes loaded for kart
	Vector3 frontLeftPos, frontRightPos, backLeftPos, backRightPos, steeringPos; // Position of the wheel and steering wheel relative to kart position

	int lap;

	SpotLight spotLights[2]; // Spotlight structures to render headlight effect

	bool isDriveGear; // Drive/Reverse gear
	double gearShiftDelay; // Bounce time for changing between drive and reverse gear
	double skillDelay;
	double buffDelay;
	int Present;
	int axescount;
	const float * axes;
	int buttonscount;
	const unsigned char* buttons;
};

#endif // !KART_H
