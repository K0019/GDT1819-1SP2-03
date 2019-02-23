#include "c_m_Player.h"

int c_m_Player::Player_ID = 0;


c_m_Player::c_m_Player(unsigned int uSpotLight)
{
	if (Player_ID == NULL) {
		Player_ID = 1;
	}
	else
		Player_ID++;

	myPlayer_ID = Player_ID;

	Cam = Camera();
	car = new Kart(MeshBuilder::GenerateOBJ("OBJ//basic_kart.obj", "Image//basic_texture.tga", type::SHADER_3),
		MeshBuilder::GenerateOBJ("OBJ//pikachu_kart.obj", "Image//pikachu_texture.tga", type::SHADER_3),
		MeshBuilder::GenerateOBJ("OBJ//eevee_kart.obj", "Image//eevee_texture.tga", type::SHADER_3),
		MeshBuilder::GenerateOBJ("OBJ//mew_kart.obj", "Image//mew_texture.tga", type::SHADER_3),
		MeshBuilder::GenerateOBJ("OBJ//squirtle_kart.obj", "Image//squirtle_texture.tga", type::SHADER_3),
		MeshBuilder::GenerateOBJ("OBJ//basic_wheel.obj", "Image//basic_wheel_texture.tga", type::SHADER_3),
		MeshBuilder::GenerateOBJ("OBJ//basic_wheel.obj", "Image//pikachu_wheel_texture.tga", type::SHADER_3),
		MeshBuilder::GenerateOBJ("OBJ//basic_wheel.obj", "Image//eevee_wheel_texture.tga", type::SHADER_3),
		MeshBuilder::GenerateOBJ("OBJ//basic_wheel.obj", "Image//mew_wheel_texture.tga", type::SHADER_3),
		MeshBuilder::GenerateOBJ("OBJ//basic_wheel.obj", "Image//squirtle_wheel_texture.tga", type::SHADER_3),
		MeshBuilder::GenerateOBJ("OBJ//SteeringWheel.obj", "Image//Steering_wheel_texture.tga", type::SHADER_3),
		Vector3(2.68f, 1.24f, 1.59f), // Front left
		Vector3(-2.68f, 1.24f, 1.59f), // Front right
		Vector3(3.28f, 1.24f, -1.43f), // Back left
		Vector3(-3.28f, 1.24f, -1.43f), // Back right
		Vector3(0.0f, 0.368f, 1.974f),
		uSpotLight,
		OBB(Vector3(0.0f, 4.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), 2.0f, 2.0f, 2.0f)); // Steering wheel

	ModGate::detector.registerKart(car);
}


c_m_Player::~c_m_Player()
{
	ModGate::detector.removeKart(car);
	delete car;

	--Player_ID;
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

void c_m_Player::render(unsigned int uMatrixMVS) const
{
	car->render(uMatrixMVS);
}

void c_m_Player::reset()
{
}

const Camera & c_m_Player::getCam() const
{
	// TODO: insert return statement here
	return Cam;
}

const Kart* c_m_Player::getCar() const
{
	return car;
}