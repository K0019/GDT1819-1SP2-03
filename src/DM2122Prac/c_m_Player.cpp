#include "c_m_Player.h"




c_m_Player::c_m_Player()
{
	if (Player_ID == NULL) {
		Player_ID = 1;
	}
	else
		Player_ID++;

	myPlayer_ID = Player_ID;

}


c_m_Player::~c_m_Player()
{
}

void c_m_Player::update(GLFWwindow * window, double deltaTime, unsigned int uSpotLight)
{
	// Update the kart
	car->update(window, deltaTime, uSpotLight,myPlayer_ID);

	// Update camera
	Cam.pos = car->getPos();
	double effectiveYaw = car->getYaw();
	//// Camera position rotation
	//if (isPressed(window, GLFW_KEY_DOWN))
	//	effectiveYaw += 180.0;
	//else if (isPressed(window, GLFW_KEY_LEFT))
	//	effectiveYaw += 90.0;
	//else if (isPressed(window, GLFW_KEY_RIGHT))
	//	effectiveYaw += 270.0;

	Cam.pos.x -= sinf(Math::DegreeToRadian(static_cast<float>(effectiveYaw))) * 12.0f;
	Cam.pos.y += 8.0f;
	Cam.pos.z -= cosf(Math::DegreeToRadian(static_cast<float>(effectiveYaw))) * 12.0f;

	Cam.front = (Vector3(car->getPos().x, car->getPos().y + 5.0f, car->getPos().z) - Cam.pos).Normalize();
}

void c_m_Player::reset()
{
}

const Camera & c_m_Player::getCam() const
{
	// TODO: insert return statement here
	return Cam;
}
