#include "Kart.h"
#include <chrono>

bool Kart::player2_invert_control = false;
bool Kart::player1_invert_control = false;
bool Kart::player2_slow = false;
bool Kart::player1_slow = false;
bool Kart::player1_stun = false;
bool Kart::player2_stun = false;


// Constructor
Kart::Kart(Mesh* basic, Mesh* pikachu, Mesh* eevee, Mesh*mew, Mesh*squirtle,
	Mesh* basic_wheel, Mesh* pikachu_wheel, Mesh* eevee_wheel, Mesh* mew_wheel, Mesh* squirtle_wheel, Mesh* steeringWheel,
	const Vector3& wheelFrontLeftPos, const Vector3& wheelFrontRightPos,
	const Vector3& wheelBackLeftPos, const Vector3& wheelBackRightPos,
	const Vector3& steeringWheelPos, unsigned int uSpotLight, const OBB& obb , const Vector3& pos1, int ID)
	: MovingPhysicsObject(obb)
	, pos(pos1)
	, yaw(90.0)
	, pitch(0.0)
	, roll(0.0)
	, speed(0.0)
	, turnForce(0.0)
	, wheelRotation(0.0)
	, isDriveGear(true)
	, gearShiftDelay(0.0)
	, lap(1)
	, basic(basic)
	, pikachu(pikachu)
	, eevee(eevee)
	, mew(mew)
	, squirtle(squirtle)
	, basic_wheel(basic_wheel)
	, pikachu_wheel(pikachu_wheel)
	, eevee_wheel(eevee_wheel)
	, mew_wheel(mew_wheel)
	, squirtle_wheel(squirtle_wheel)
	, steeringWheel(steeringWheel)
	, frontLeftPos(wheelFrontLeftPos)
	, frontRightPos(wheelFrontRightPos)
	, backLeftPos(wheelBackLeftPos)
	, backRightPos(wheelBackRightPos)
	, steeringPos(steeringWheelPos)
	, ID(ID)
{
	// Initialize spotlights
	for (int i = 0; i < 2; ++i)
	{
		spotLights[i].position = Vector3(0.0f, 0.0f, 0.0f);
		spotLights[i].ambient = Vector3(0.0f, 0.0f, 0.0f);
		spotLights[i].diffuse = Vector3(0.7f, 0.7f, 0.7f);
		spotLights[i].specular = Vector3(0.3f, 0.3f, 0.3f);
		spotLights[i].constant = 0.8f;
		spotLights[i].linear = 0.009f;
		spotLights[i].quadratic = 0.0f;
		spotLights[i].direction = Vector3(1.0f, 0.0f, 0.0f);
		spotLights[i].cosInner = cosf(Math::DegreeToRadian(35.0f));
		spotLights[i].cosOuter = cosf(Math::DegreeToRadian(50.0f));
	}

	MS model;
	model.LoadIdentity();

	// Rotation of kart
	model.PushMatrix(); // 1
	model.Translate(pos.x, pos.y, pos.z);
	model.Rotate(static_cast<float>(yaw), 0.0f, 1.0f, 0.0f);
	model.Translate(0.0f, 2.5f, 4.5f);

	// Right headlight
	model.PushMatrix(); // 2
	model.Translate(1.8f, 0.0f, 0.0f);
	spotLights[0].position = model.Top() * Vector3();
	model.PopMatrix(); // 2

	// Left headlight
	model.PushMatrix(); // 2
	model.Translate(-1.8f, 0.0f, 0.0f);
	spotLights[1].position = model.Top() * Vector3();
	model.PopMatrix(); // 2

	model.PopMatrix(); // 1

	// Set spotlight direction
	spotLights[0].direction = spotLights[1].direction = Vector3(cosf(Math::DegreeToRadian(static_cast<float>(pitch))) * cosf(Math::DegreeToRadian(static_cast<float>(-yaw + 90.0f))),
																sinf(Math::DegreeToRadian(static_cast<float>(pitch))),
																cosf(Math::DegreeToRadian(static_cast<float>(pitch))) * sinf(Math::DegreeToRadian(static_cast<float>(-yaw + 90.0f))));

	// Update opengl spotlights
	glBindBuffer(GL_UNIFORM_BUFFER, uSpotLight);
	for (int i = (ID << 1); i < 2 + (ID << 1); ++i)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i, 12, &spotLights[i - (ID << 1)].position.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 12, 4, &spotLights[i - (ID << 1)].constant);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 16, 4, &spotLights[i - (ID << 1)].linear);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 20, 4, &spotLights[i - (ID << 1)].quadratic);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 32, 12, &spotLights[i - (ID << 1)].ambient.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 48, 12, &spotLights[i - (ID << 1)].diffuse.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 64, 12, &spotLights[i - (ID << 1)].specular.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 80, 12, &spotLights[i - (ID << 1)].direction.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 92, 4, &spotLights[i - (ID << 1)].cosInner);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 96, 4, &spotLights[i - (ID << 1)].cosOuter);
	}

	srand((unsigned)time(NULL));

	m_status = e_basic;
	m_buff = b_nothing;


	Present = glfwJoystickPresent(GLFW_JOYSTICK_1);
	std::cout << Present;
	
}

// Destructor
Kart::~Kart()
{
	// Delete meshes
	delete basic,
	delete pikachu,
	delete mew,
	delete squirtle,
	delete eevee;
	delete basic_wheel;
	delete eevee_wheel;
	delete pikachu_wheel;
	delete mew_wheel;
	delete squirtle_wheel;
	delete steeringWheel;
}

// Move the kart and handle input
void Kart::update(GLFWwindow* window, double deltaTime)
{
	if (Present == 1) {
		axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axescount);

		std::cout << "No of axes " << axes[0] << std::endl;
		//std::cout << "Lefttrack pad X axis" << axes[0] << std::endl;
		//std::cout << "Lefttrack pad y axis" << axes[1] << std::endl;
		/*	std::cout <<"Righttrack pad x axis"<< axes[2] << std::endl;
		std::cout <<"Righttrack pad y axis"<< axes[3] << std::endl;
		std::cout <<"L2"<< axes[4] << std::endl;
		std::cout <<"R2"<< axes[5] << std::endl;*/


		buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonscount);
		/*std::cout << "no of buttons : " << buttonscount << std :: endl;*/
		/*if (GLFW_PRESS == buttons[0]) {
		std::cout << " A button pressed" << std::endl;
		}
		if (GLFW_PRESS == buttons[1]) {
		std::cout << "B pressed" << std::endl;
		}
		if (GLFW_PRESS == buttons[2]) {
		std::cout << "X pressed" << std::endl;
		}
		if (GLFW_PRESS == buttons[3]) {
		std::cout << "Y pressed" << std::endl;
		}
		if (GLFW_PRESS == buttons[4]) {
		std::cout << " L1 pressed" << std::endl;
		}
		if (GLFW_PRESS == buttons[5]) {
		std::cout << " R1 pressed" << std::endl;
		}
		if (GLFW_PRESS == buttons[6]) {
		std::cout << " Back pressed" << std::endl;
		}
		if (GLFW_PRESS == buttons[7]) {
		std::cout << " start pressed" << std::endl;
		}
		if (GLFW_PRESS == buttons[8]) {
		std::cout << " Left Track pad is pressed" << std::endl;
		}if (GLFW_PRESS == buttons[9]) {
		std::cout << " Right Track pad is pressed" << std::endl;
		}if (GLFW_PRESS == buttons[10]) {
		std::cout << " UP is pressed" << std::endl;
		}if (GLFW_PRESS == buttons[11]) {
		std::cout << "  Right is pressed" << std::endl;
		}if (GLFW_PRESS == buttons[12]) {
		std::cout << "  Down is pressed" << std::endl;
		}if (GLFW_PRESS == buttons[13]) {
		std::cout << "  Left is pressed" << std::endl;
		}*/

		const char* name = glfwGetJoystickName(GLFW_JOYSTICK_1);

	}

	// Gear shift bounce time
	gearShiftDelay -= deltaTime;
	// Speed up/down
	if (gearShiftDelay <= 0.0 && isPressed(window, GLFW_KEY_W))
	{
		if (isDriveGear) // Driving
		{
			speed += 50.0 * deltaTime;
		}
		else // Reverse
		{
			speed += 50.0 * deltaTime;
			if (speed > 0.0) // Check gear change
			{
				isDriveGear = true;
				gearShiftDelay = 0.15;
				speed = 0.0;
			}
		}

	}
	if (gearShiftDelay <= 0.0 && isPressed(window, GLFW_KEY_S))
	{
		if (!isDriveGear) // Reverse
		{
			speed -= 21.0 * deltaTime;
		}
		else // Driving
		{
			speed -= 50.0 * deltaTime;
			if (speed < 0.0) // Check gear change
			{
				isDriveGear = false;
				gearShiftDelay = 0.15;
				speed = 0.0;
			}
		}
	}
	// Turn input
	if (isPressed(window, GLFW_KEY_A))
	{
		turnForce += 6.0 * deltaTime;

	}
	if (isPressed(window, GLFW_KEY_D))
	{
		turnForce -= 6.0 * deltaTime;
	}
	if (Present == 1) {
		// analog control
		if (gearShiftDelay <= 0.0 && (axes[1] > 0))
		{
			if (isDriveGear) // Driving
			{
				speed += axes[1] * 50.0 * deltaTime;
			}
			else // Reverse
			{
				speed += axes[1] * 50.0 * deltaTime;
				if (speed > 0.0) // Check gear change
				{
					isDriveGear = true;
					gearShiftDelay = 0.15;
					speed = 0.0;
				}
			}
		}
		if (gearShiftDelay <= 0.0 && (axes[1] < 0))
		{
			if (!isDriveGear) // Reverse
			{
				speed -= -axes[1] * 21.0 * deltaTime;
			}
			else // Driving
			{
				speed -= -axes[1] * 50.0 * deltaTime;
				if (speed < 0.0) // Check gear change
				{
					isDriveGear = false;
					gearShiftDelay = 0.15;
					speed = 0.0;
				}
			}
		}
		// Turn input
		if (axes[2] < 0)
		{
			turnForce += -axes[2] * 6.0 * deltaTime;

		}
		if (axes[2] > 0)
		{
			turnForce -= axes[2] * 6.0 * deltaTime;
		}
	}
	// Friction / Air resistance
	if (speed > 0.0)
	{
		speed -= 3.0 * deltaTime + speed * 0.4 * deltaTime;
		if (speed < 0.0)
			speed = 0.0;
	}
	else if (speed < 0.0)
	{
		speed += 3.0 * deltaTime - speed * 0.4 * deltaTime;
		if (speed > 0.0)
			speed = 0.0;
	}
	else
	{
		speed = 0.0;
	}

	// Turnforce decay
	turnForce *= 0.9;
	if (turnForce < 0.05 && turnForce > -0.05)
		turnForce = 0.0;
	
	// Turnforce clamp
	if (turnForce < -6.0)
		turnForce = -6.0;
	else if (turnForce > 6.0)
		turnForce = 6.0;

	// Calculate Rotation
	if (speed != 0.0)
	{
		double turnDegreeLinear = fabs(turnForce * (speed / 60.0) * 3.5);
		double turnDegreeQuadratic = fabs(turnForce / (speed / 60.0) * 0.7);
		double turnDegree = ((turnDegreeLinear < turnDegreeQuadratic) ? (turnDegreeLinear) : (turnDegreeQuadratic));
		if (turnForce < 0.0) // Rotate left
			turnDegree = -turnDegree;
		if (speed < 0.0) // Reverse
			turnDegree = -turnDegree;

		yaw += turnDegree;
		rotateCollision(static_cast<float>(turnDegree), Vector3(0.0f, 1.0f, 0.0f));
	}

	// Calculate velocity
	velocity = Vector3(sinf(Math::DegreeToRadian(static_cast<float>(yaw))), 0.0f, cosf(Math::DegreeToRadian(static_cast<float>(yaw)))) * static_cast<float>(speed);

	// Wheel rotation
	if (speed >= 0.0)
		wheelRotation += velocity.Length() * 40.0f * deltaTime;
	else
		wheelRotation -= velocity.Length() * 40.0f * deltaTime;
	// Keep rotation within 0 - 360
	if (wheelRotation > 360.0f)
		wheelRotation -= 360.0f;
	else if (wheelRotation < 0.0f)
		wheelRotation += 360.0f;

	// Move
	pos += velocity * static_cast<float>(deltaTime);
	//pos.y -= 0.5f;
	if (pos.y < 0.0f)
		pos.y = 0.0f;
	// Update OBB
	setCollisionPosition(Vector3(pos.x, pos.y + 3.0f, pos.z));
}

void Kart::updateOpenGL(unsigned int uSpotLight)
{
	// Spotlight
	MS model;
	model.LoadIdentity();

	// Translation
	model.PushMatrix(); // 1
	model.Translate(pos.x, pos.y, pos.z);
	model.Rotate(static_cast<float>(yaw), 0.0f, 1.0f, 0.0f);
	model.Translate(0.0f, 2.5f, 4.5f);
	// Right headlight
	model.PushMatrix(); // 2
	model.Translate(1.8f, 0.0f, 0.0f);
	spotLights[0].position = model.Top() * Vector3();
	model.PopMatrix(); // 2
	// Left headlight
	model.PushMatrix(); // 2
	model.Translate(-1.8f, 0.0f, 0.0f);
	spotLights[1].position = model.Top() * Vector3();
	model.PopMatrix(); // 2

	model.PopMatrix(); // 1

	// Set spotlight direction
	spotLights[0].direction = spotLights[1].direction = Vector3(cosf(Math::DegreeToRadian(static_cast<float>(pitch))) * cosf(Math::DegreeToRadian(static_cast<float>(-yaw + 90.0f))),
		sinf(Math::DegreeToRadian(static_cast<float>(pitch))),
		cosf(Math::DegreeToRadian(static_cast<float>(pitch))) * sinf(Math::DegreeToRadian(static_cast<float>(-yaw + 90.0f))));

	// Update opengl spotlights
	glBindBuffer(GL_UNIFORM_BUFFER, uSpotLight);
	for (int i = (ID << 1); i < 2 + (ID << 1); ++i)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i, 12, &spotLights[i - (ID << 1)].position.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 12, 4, &spotLights[i - (ID << 1)].constant);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 16, 4, &spotLights[i - (ID << 1)].linear);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 20, 4, &spotLights[i - (ID << 1)].quadratic);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 32, 12, &spotLights[i - (ID << 1)].ambient.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 48, 12, &spotLights[i - (ID << 1)].diffuse.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 64, 12, &spotLights[i - (ID << 1)].specular.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 80, 12, &spotLights[i - (ID << 1)].direction.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 92, 4, &spotLights[i - (ID << 1)].cosInner);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 96, 4, &spotLights[i - (ID << 1)].cosOuter);
	}
}

void Kart::update(GLFWwindow * window, double deltaTime, unsigned int uSpotLight, int PlayerID)
{
	if (Present == 1) {
		axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axescount);

		//std::cout << "No of axes " << axescount << std::endl;
		//std::cout << "Lefttrack pad X axis" << axes[0] << std::endl;
		//std::cout << "Lefttrack pad y axis" << axes[1] << std::endl;
		/*	std::cout <<"Righttrack pad x axis"<< axes[2] << std::endl;
		std::cout <<"Righttrack pad y axis"<< axes[3] << std::endl;
		std::cout <<"L2"<< axes[4] << std::endl;
		std::cout <<"R2"<< axes[5] << std::endl;*/


		buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonscount);
		/*std::cout << "no of buttons : " << buttonscount << std :: endl;*/
		/*if (GLFW_PRESS == buttons[0]) {
		std::cout << " A button pressed" << std::endl;
		}
		if (GLFW_PRESS == buttons[1]) {
		std::cout << "B pressed" << std::endl;
		}
		if (GLFW_PRESS == buttons[2]) {
		std::cout << "X pressed" << std::endl;
		}
		if (GLFW_PRESS == buttons[3]) {
		std::cout << "Y pressed" << std::endl;
		}
		if (GLFW_PRESS == buttons[4]) {
		std::cout << " L1 pressed" << std::endl; ///////
		}
		if (GLFW_PRESS == buttons[5]) {
		std::cout << " R1 pressed" << std::endl;
		}
		if (GLFW_PRESS == buttons[6]) {
		std::cout << " Back pressed" << std::endl;
		}
		if (GLFW_PRESS == buttons[7]) {
		std::cout << " start pressed" << std::endl;
		}
		if (GLFW_PRESS == buttons[8]) {
		std::cout << " Left Track pad is pressed" << std::endl;
		}if (GLFW_PRESS == buttons[9]) {
		std::cout << " Right Track pad is pressed" << std::endl;
		}if (GLFW_PRESS == buttons[10]) {
		std::cout << " UP is pressed" << std::endl;
		}if (GLFW_PRESS == buttons[11]) {
		std::cout << "  Right is pressed" << std::endl;
		}if (GLFW_PRESS == buttons[12]) {
		std::cout << "  Down is pressed" << std::endl;
		}if (GLFW_PRESS == buttons[13]) {
		std::cout << "  Left is pressed" << std::endl;
		}*/

		const char* name = glfwGetJoystickName(GLFW_JOYSTICK_1);

	}
	if (PlayerID == 1) {
		// Gear shift bounce time
		gearShiftDelay -= deltaTime;
		skillDelay -= deltaTime;
		
		// Speed up/down
		if (gearShiftDelay <= 0.0 && isPressed(window, GLFW_KEY_W))
		{
			if (Kart::getInvert1() == false)
			{
				if (isDriveGear) // Driving
				{
					speed += 50.0 * deltaTime;
				}
				else // Reverse
				{
					speed += 50.0 * deltaTime;
					if (speed > 0.0) // Check gear change
					{
						isDriveGear = true;
						gearShiftDelay = 0.15;
						speed = 0.0;
					}
				}
			}
			if (Kart::getInvert1() == true)
			{
				if (!isDriveGear) // Reverse
				{
					speed -= 21.0 * deltaTime;
				}
				else // Driving
				{
					speed -= 50.0 * deltaTime;
					if (speed < 0.0) // Check gear change
					{
						isDriveGear = false;
						gearShiftDelay = 0.15;
						speed = 0.0;
					}
				}

			}
			music::player.playsound("");
		}
		if (gearShiftDelay <= 0.0 && isPressed(window, GLFW_KEY_S))
		{
			if (Kart::getInvert1() == false)
			{
				if (!isDriveGear) // Reverse
				{
					speed -= 21.0 * deltaTime;
				}
				else // Driving
				{
					speed -= 50.0 * deltaTime;
					if (speed < 0.0) // Check gear change
					{
						isDriveGear = false;
						gearShiftDelay = 0.15;
						speed = 0.0;
					}
				}
			}
			else
			{
				if (isDriveGear) // Driving
				{
					speed += 50.0 * deltaTime;
				}
				else // Reverse
				{
					speed += 50.0 * deltaTime;
					if (speed > 0.0) // Check gear change
					{
						isDriveGear = true;
						gearShiftDelay = 0.15;
						speed = 0.0;
					}
				}
			}
			music::player.playsound("");
		}
		// Turn input
		if (isPressed(window, GLFW_KEY_A))
		{
			if (Kart::getInvert1() == false)
			{
				turnForce += 6.0 * deltaTime;
			}
			if (Kart::getInvert1() == true)
			{
				turnForce -= 6.0 * deltaTime;
			}
			music::player.playsound("");
		}
		if (isPressed(window, GLFW_KEY_D))
		{
			if (Kart::getInvert1() == false)
			{
				turnForce -= 6.0 * deltaTime;
			}
			if (Kart::getInvert1() == true)
			{
				turnForce += 6.0 * deltaTime;
			}
			music::player.playsound("");
		}
		//use skill
		if (isPressed(window, GLFW_KEY_Q))
		{
			std::cout << "here" << std::endl;
			switch (m_status)
			{
			case e_basic:
				break;
			case e_pikachu:
				if (player_used == false)
				{
					player2_stun = true;
					skillDelay = 0.5;
					player_used = true;
				}
				music::player.playsound("");
				break;
			case e_eevee:
				if (player_used == false)
				{
					player_eevee_up = true;
					skillDelay = 1.5;
					player_used = true;
				}
				music::player.playsound("");
				break;
			case e_mew:
				if (player_used == false)
				{
					player2_invert_control = true;
					skillDelay = 3.5;
					player_used = true;
				}
				music::player.playsound("");
				break;
			case e_squirtle:
				if (player_used == false)
				{
					player2_slow = true;
					skillDelay = 1.5;
					std::cout << "here" << std::endl;
					player_used = true;
				}
				music::player.playsound("");
				break;
			default:
				player_used = false;
					break;
			}
		}
		//eevee speed up
		
		
		

		if (player2_invert_control == true)
		{

			if (skillDelay <= 0.0)
			{
				player2_invert_control = false;
			}
			music::player.playsound("");
		}
		if (player2_slow == true)
		{

			if (skillDelay <= 0.0)
			{
				player2_slow = false;
			}
			music::player.playsound("");
		}
		if (player2_stun == true)
		{

			if (skillDelay <= 0.0)
			{
				player2_stun = false;
			}
			music::player.playsound("");
		}

		if (player_eevee_up == true)
		{

			speed += 75 * deltaTime;
			m_buff = b_speed;
			if (skillDelay <= 0.0)
			{
				player_eevee_up = false;
			}
		}
		else if (getStun1() == true)
		{
			m_buff = b_stun;
			speed = 0;
		}
		else if (getSlow1() == true)
		{
			m_buff = b_slow;
			speed -= 40 * deltaTime;
		}
		else if (player1_invert_control == true)
		{
			m_buff = b_confuse;
		}
		else { m_buff = b_nothing; }





		// Friction / Air resistance
		if (speed > 0.0)
		{
			speed -= 3.0 * deltaTime + speed * 0.4 * deltaTime;
			if (speed < 0.0)
				speed = 0.0;
		}
		else if (speed < 0.0)
		{
			speed += 3.0 * deltaTime - speed * 0.4 * deltaTime;
			if (speed > 0.0)
				speed = 0.0;
		}
		else
		{
			speed = 0.0;
		}

		// Turnforce decay
		turnForce *= 0.9;
		if (turnForce < 0.05 && turnForce > -0.05)
		{
			turnForce = 0.0;
		}
		// Turnforce clamp
		if (turnForce < -6.0)
		{
			turnForce = -6.0;
		}
		else if (turnForce > 6.0)
		{

			turnForce = 6.0;
		}
		// Calculate Rotation
		if (speed != 0.0)
		{
			double turnDegreeLinear = fabs(turnForce * (speed / 60.0) * 3.5);
			double turnDegreeQuadratic = fabs(turnForce / (speed / 60.0) * 0.7);
			double turnDegree = ((turnDegreeLinear < turnDegreeQuadratic) ? (turnDegreeLinear) : (turnDegreeQuadratic));
			if (turnForce < 0.0) // Rotate left
				turnDegree = -turnDegree;
			if (speed < 0.0) // Reverse
				turnDegree = -turnDegree;

			yaw += turnDegree;
		}

		// Calculate velocity
		velocity = Vector3(sinf(Math::DegreeToRadian(static_cast<float>(yaw))), 0.0f, cosf(Math::DegreeToRadian(static_cast<float>(yaw)))) * static_cast<float>(speed);

		// Wheel rotation
		if (speed >= 0.0)
		{
			wheelRotation += velocity.Length() * 40.0f * deltaTime;
		}
		else
		{

			wheelRotation -= velocity.Length() * 40.0f * deltaTime;
		}
		// Keep rotation within 0 - 360
		if (wheelRotation > 360.0f)
		{

			wheelRotation -= 360.0f;
		}
		else if (wheelRotation < 0.0f)
		{

			wheelRotation += 360.0f;
		}




		// Move
		pos += velocity * static_cast<float>(deltaTime);

		// Spotlight
		MS model;
		model.LoadIdentity();

		// Translation
		model.PushMatrix(); // 1
		model.Translate(pos.x, pos.y, pos.z);
		model.Rotate(static_cast<float>(yaw), 0.0f, 1.0f, 0.0f);
		model.Translate(0.0f, 2.5f, 4.5f);
		// Right headlight
		model.PushMatrix(); // 2
		model.Translate(1.8f, 0.0f, 0.0f);
		spotLights[0].position = model.Top() * Vector3();
		model.PopMatrix(); // 2
		// Left headlight
		model.PushMatrix(); // 2
		model.Translate(-1.8f, 0.0f, 0.0f);
		spotLights[1].position = model.Top() * Vector3();
		model.PopMatrix(); // 2

		model.PopMatrix(); // 1

		// Set spotlight direction
		spotLights[0].direction = spotLights[1].direction = Vector3(cosf(Math::DegreeToRadian(static_cast<float>(pitch))) * cosf(Math::DegreeToRadian(static_cast<float>(-yaw + 90.0f))),
			sinf(Math::DegreeToRadian(static_cast<float>(pitch))),
			cosf(Math::DegreeToRadian(static_cast<float>(pitch))) * sinf(Math::DegreeToRadian(static_cast<float>(-yaw + 90.0f))));

		// Update opengl spotlights
		glBindBuffer(GL_UNIFORM_BUFFER, uSpotLight);
		for (int i = (ID << 1); i < 2 + (ID << 1); ++i)
		{
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i, 12, &spotLights[i - (ID << 1)].position.x);
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 12, 4, &spotLights[i - (ID << 1)].constant);
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 16, 4, &spotLights[i - (ID << 1)].linear);
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 20, 4, &spotLights[i - (ID << 1)].quadratic);
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 32, 12, &spotLights[i - (ID << 1)].ambient.x);
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 48, 12, &spotLights[i - (ID << 1)].diffuse.x);
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 64, 12, &spotLights[i - (ID << 1)].specular.x);
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 80, 12, &spotLights[i - (ID << 1)].direction.x);
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 92, 4, &spotLights[i - (ID << 1)].cosInner);
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 96, 4, &spotLights[i - (ID << 1)].cosOuter);
		}

		// Update OBB
		setCollisionPosition(Vector3(pos.x, pos.y + 3.0f, pos.z));
	}
	else // player 2 
	{
		// Gear shift bounce time
		gearShiftDelay -= deltaTime;
		skillDelay -= deltaTime;

		// Speed up/down
		if (gearShiftDelay <= 0.0 && (isPressed(window, GLFW_KEY_U)))
		{
			if (Kart::getInvert2() == false)
			{
				if (isDriveGear) // Driving
				{
					speed += 50.0 * deltaTime;
				}
				else // Reverse
				{
					speed += 50.0 * deltaTime;
					if (speed > 0.0) // Check gear change
					{
						isDriveGear = true;
						gearShiftDelay = 0.15;
						speed = 0.0;
					}
				}
			}
			if (Kart::getInvert2() == true)
			{
				if (isDriveGear) // Driving
				{
					speed -= 50.0 * deltaTime;
				}
				else // Reverse
				{
					speed -= 50.0 * deltaTime;
					if (speed < 0.0) // Check gear change
					{
						isDriveGear = false;
						gearShiftDelay = 0.15;
						speed = 0.0;
					}
				}
			}
		}
		if (gearShiftDelay <= 0.0 && isPressed(window, GLFW_KEY_J))
		{
			if (Kart::getInvert2() == false)
			{
				if (!isDriveGear) // Reverse
				{
					speed -= 21.0 * deltaTime;
				}
				else // Driving
				{
					speed -= 50.0 * deltaTime;
					if (speed < 0.0) // Check gear change
					{
						isDriveGear = false;
						gearShiftDelay = 0.15;
						speed = 0.0;
					}
				}
			}
			if (Kart::getInvert2() == true)
			{
				if (!isDriveGear) // Reverse
				{
					speed += 21.0 * deltaTime;
				}
				else // Driving
				{
					speed += 50.0 * deltaTime;
					if (speed > 0.0) // Check gear change
					{
						isDriveGear = true;
						gearShiftDelay = 0.15;
						speed = 0.0;
					}
				}
			}
		}
		// Turn input
		if (isPressed(window, GLFW_KEY_H))
		{
			if (Kart::getInvert2() == false)
			{
				turnForce += 6.0 * deltaTime;
			}
			if (Kart::getInvert2() == true)
			{
				turnForce -= 6.0 * deltaTime;
			}


		}
		if (isPressed(window, GLFW_KEY_K))
		{
			if (Kart::getInvert2() == false)
			{
				turnForce -= 6.0 * deltaTime;
			}
			if (Kart::getInvert2() == true)
			{
				turnForce += 6.0 * deltaTime;
			}
		}
		if (isPressed(window, GLFW_KEY_I))
		{
			std::cout << "here" << std::endl;
			switch (m_status)
			{
			case e_basic:
				break;
			case e_pikachu:
				if (player_used == false)
				{
					player1_stun = true;
					skillDelay = 0.5;
					player_used = true;
				}
				break;
			case e_eevee:
				if (player_used == false)
				{
					player_eevee_up = true;
					skillDelay = 1.5;
					player_used = true;
				}
				break;
			case e_mew:
				if (player_used == false)
				{
					player1_invert_control = true;
					skillDelay = 3.5;
					player_used = true;
				}
				break;
			case e_squirtle:
				if (player_used == false)
				{
					player1_slow = true;
					skillDelay = 1.5;
					std::cout << "here" << std::endl;
					player_used = true;
				}
				break;
			default:
				player_used = false;
				break;
			}
		}

		// analog control
		if (Present == 1)
		{
			if (Kart::getInvert2() == false)
			{
				if (gearShiftDelay <= 0.0 && (axes[1] > 0))
				{

					if (isDriveGear) // Driving
					{
						speed += axes[1] * 50.0 * deltaTime;
					}
					else // Reverse
					{
						speed += axes[1] * 50.0 * deltaTime;
						if (speed > 0.0) // Check gear change
						{
							isDriveGear = true;
							gearShiftDelay = 0.15;
							speed = 0.0;
						}
					}
				}

				if (gearShiftDelay <= 0.0 && (axes[1] < 0))
				{
					if (!isDriveGear) // Reverse
					{
						speed -= -axes[1] * 21.0 * deltaTime;
					}
					else // Driving
					{
						speed -= -axes[1] * 50.0 * deltaTime;
						if (speed < 0.0) // Check gear change
						{
							isDriveGear = false;
							gearShiftDelay = 0.15;
							speed = 0.0;
						}
					}
				}
				// Turn input
				if (axes[2] < 0)
				{
					turnForce += -axes[2] * 6.0 * deltaTime;

				}
				if (axes[2] > 0)
				{
					turnForce -= axes[2] * 6.0 * deltaTime;
				}
			}
			if (Kart::getInvert2() == true)
			{
				if (gearShiftDelay <= 0.0 && (axes[1] > 0))
				{


					if (!isDriveGear) // Reverse
					{
						speed += axes[1] * 21.0 * deltaTime;
					}
					else // Driving
					{
						speed += axes[1] * 50.0 * deltaTime;
						if (speed < 0.0) // Check gear change
						{
							isDriveGear = false;
							gearShiftDelay = 0.15;
							speed = 0.0;
						}
					}
				}

				if (gearShiftDelay <= 0.0 && (axes[1] < 0))
				{
					if (isDriveGear) // Driving
					{
						speed -= axes[1] * 50.0 * deltaTime;
					}
					else // Reverse
					{
						speed -= axes[1] * 50.0 * deltaTime;
						if (speed > 0.0) // Check gear change
						{
							isDriveGear = true;
							gearShiftDelay = 0.15;
							speed = 0.0;
						}
					}
				}
				// Turn input
				if (axes[2] < 0)
				{
					turnForce -= -axes[2] * 6.0 * deltaTime;

				}
				if (axes[2] > 0)
				{
					turnForce += axes[2] * 6.0 * deltaTime;
				}
			}
			if (GLFW_PRESS == buttons[4])
			{
				std::cout << "here" << std::endl;
				switch (m_status)
				{
				case e_basic:
					break;
				case e_pikachu:
					if (player_used == false)
					{
						player1_stun = true;
						skillDelay = 0.5;
						player_used = true;
					}
					break;
				case e_eevee:
					if (player_used == false)
					{
						player_eevee_up = true;
						skillDelay = 1.5;
						std::cout << "here" << std::endl;
						player_used = true;
					}
					break;
				case e_mew:
					if (player_used == false)
					{
						player1_invert_control = true;
						skillDelay = 3.5;
						std::cout << "here" << std::endl;
						player_used = true;
					}
					break;
				case e_squirtle:
					if (player_used == false)
					{
						player1_slow = true;
						skillDelay = 1.5;
						player_used = true;
					}
					break;
				default:
					player_used = false;
					break;
				}
			}
		}
		//eevee speed up
		

		//mew invert
		if (player1_invert_control == true)
		{
			
			if (skillDelay <= 0.0)
			{
				player1_invert_control = false;
			}

		}
		if (player1_slow == true)
		{
			
			if (skillDelay <= 0.0)
			{
				player1_slow = false;
			}

		}
		if (player1_stun == true)
		{

			if (skillDelay <= 0.0)
			{
				player1_stun=false;
			}

		}

		if (player_eevee_up == true)
		{

			speed += 75 * deltaTime;
			m_buff = b_speed;
			if (skillDelay <= 0.0)
			{
				player_eevee_up = false;
			}

		}
		else if (getStun2() == true)
		{
			m_buff = b_stun;
			speed = 0;
		}	
		else if (getSlow2() == true)
		{
			m_buff = b_slow;
			speed -= 40 * deltaTime;
		}		
		else if (player2_invert_control == true)
		{
			m_buff = b_confuse;
		}
		else { m_buff = b_nothing; }
		
		
		// Friction / Air resistance
		if (speed > 0.0)
		{
			speed -= 3.0 * deltaTime + speed * 0.4 * deltaTime;
			if (speed < 0.0)
				speed = 0.0;
		}
		else if (speed < 0.0)
		{
			speed += 3.0 * deltaTime - speed * 0.4 * deltaTime;
			if (speed > 0.0)
				speed = 0.0;
		}
		else
		{
			speed = 0.0;
		}

		// Turnforce decay
		turnForce *= 0.9;
		if (turnForce < 0.05 && turnForce > -0.05)
		{
			turnForce = 0.0;
		}
		// Turnforce clamp
		if (turnForce < -6.0)
		{
			turnForce = -6.0;
		}
		else if (turnForce > 6.0)
		{

			turnForce = 6.0;
		}
		// Calculate Rotation
		if (speed != 0.0)
		{
			double turnDegreeLinear = fabs(turnForce * (speed / 60.0) * 3.5);
			double turnDegreeQuadratic = fabs(turnForce / (speed / 60.0) * 0.7);
			double turnDegree = ((turnDegreeLinear < turnDegreeQuadratic) ? (turnDegreeLinear) : (turnDegreeQuadratic));
			if (turnForce < 0.0) // Rotate left
				turnDegree = -turnDegree;
			if (speed < 0.0) // Reverse
				turnDegree = -turnDegree;

			yaw += turnDegree;
			rotateCollision(static_cast<float>(turnDegree), Vector3(0.0f, 1.0f, 0.0f));
		}

		// Calculate velocity
		velocity = Vector3(sinf(Math::DegreeToRadian(static_cast<float>(yaw))), 0.0f, cosf(Math::DegreeToRadian(static_cast<float>(yaw)))) * static_cast<float>(speed);

		// Wheel rotation
		if (speed >= 0.0)
		{
			wheelRotation += velocity.Length() * 40.0f * deltaTime;
		}
		else
		{

			wheelRotation -= velocity.Length() * 40.0f * deltaTime;
		}
		// Keep rotation within 0 - 360
		if (wheelRotation > 360.0f)
		{

			wheelRotation -= 360.0f;
		}
		else if (wheelRotation < 0.0f)
		{

			wheelRotation += 360.0f;
		}




		// Move
		pos += velocity * static_cast<float>(deltaTime);

		// Spotlight
		MS model;
		model.LoadIdentity();

		// Translation
		model.PushMatrix(); // 1
		model.Translate(pos.x, pos.y, pos.z);
		model.Rotate(static_cast<float>(yaw), 0.0f, 1.0f, 0.0f);
		model.Translate(0.0f, 2.5f, 4.5f);
		// Right headlight
		model.PushMatrix(); // 2
		model.Translate(1.8f, 0.0f, 0.0f);
		spotLights[0].position = model.Top() * Vector3();
		model.PopMatrix(); // 2
						   // Left headlight
		model.PushMatrix(); // 2
		model.Translate(-1.8f, 0.0f, 0.0f);
		spotLights[1].position = model.Top() * Vector3();
		model.PopMatrix(); // 2

		model.PopMatrix(); // 1

						   // Set spotlight direction
		spotLights[0].direction = spotLights[1].direction = Vector3(cosf(Math::DegreeToRadian(static_cast<float>(pitch))) * cosf(Math::DegreeToRadian(static_cast<float>(-yaw + 90.0f))),
			sinf(Math::DegreeToRadian(static_cast<float>(pitch))),
			cosf(Math::DegreeToRadian(static_cast<float>(pitch))) * sinf(Math::DegreeToRadian(static_cast<float>(-yaw + 90.0f))));

		// Update opengl spotlights
		glBindBuffer(GL_UNIFORM_BUFFER, uSpotLight);
		for (int i = (ID << 1); i < 2 + (ID << 1); ++i)
		{
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i, 12, &spotLights[i - (ID << 1)].position.x);
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 12, 4, &spotLights[i - (ID << 1)].constant);
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 16, 4, &spotLights[i - (ID << 1)].linear);
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 20, 4, &spotLights[i - (ID << 1)].quadratic);
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 32, 12, &spotLights[i - (ID << 1)].ambient.x);
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 48, 12, &spotLights[i - (ID << 1)].diffuse.x);
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 64, 12, &spotLights[i - (ID << 1)].specular.x);
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 80, 12, &spotLights[i - (ID << 1)].direction.x);
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 92, 4, &spotLights[i - (ID << 1)].cosInner);
			glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 96, 4, &spotLights[i - (ID << 1)].cosOuter);
		}

		// Update OBB
		setCollisionPosition(Vector3(pos.x, pos.y + 3.0f, pos.z));
	}
}

// Render the kart
void Kart::render(unsigned int uMatrixMVS) const
{
	MS model;
	model.LoadIdentity();

	

	// Body
	model.PushMatrix(); // 1
	model.Translate(pos.x, pos.y + 0.4f, pos.z);
	//model.Rotate(roll, static_cast<float>(sin(yaw)), static_cast<float>(sin(pitch)), static_cast<float>(cos(yaw)));
	model.Rotate(static_cast<float>(yaw), 0.0f, 1.0f, 0.0f);
	model.Rotate(static_cast<float>(pitch), 1.0f, 0.0f, 0.0f);
	model.Rotate(static_cast<float>(roll), 0.0f, 0.0f, 1.0f);
	model.Scale(0.5f,0.5f, 0.5f);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	switch (m_status)
	{
	case Kart::e_basic:
		basic->Render();
		break;
	case Kart::e_pikachu:
		pikachu->Render();
		break;
	case Kart::e_eevee:
		eevee->Render();
		break;
	case Kart::e_mew:
		mew->Render();
		break;
	case Kart::e_squirtle:
		squirtle->Render();
		break;
	default:
		basic->Render();
		break;
	}

	
	// Front left wheel
	model.PushMatrix(); // 2
	model.Translate(frontLeftPos.x-0.5f, frontLeftPos.y, frontLeftPos.z);
	model.Rotate(static_cast<float>(turnForce) * 25.0f, 0.0f, 1.0f, 0.0f);
	model.Translate(0.5f, 0, 0);
	model.Rotate(static_cast<float>(wheelRotation), 1.0f, 0.0f, 0.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	switch (m_status)
	{
	case Kart::e_basic:
		basic_wheel->Render();
		break;
	case Kart::e_pikachu:
		pikachu_wheel->Render();
		break;
	case Kart::e_eevee:
		eevee_wheel->Render();
		break;
	case Kart::e_mew:
		mew_wheel->Render();
		break;
	case Kart::e_squirtle:
		squirtle_wheel->Render();
		break;
	default:
		basic_wheel->Render();
		break;
	}
	model.PopMatrix(); // 2

	// Front right wheel
	model.PushMatrix(); // 2
	model.Translate(frontRightPos.x + 0.5f, frontRightPos.y, frontRightPos.z);
	model.Rotate(180.0f + static_cast<float>(turnForce) * 25.0f, 0.0f, 1.0f, 0.0f);
	model.Translate(0.5f, 0, 0);
	model.Rotate(static_cast<float>(-wheelRotation), 1.0f, 0.0f, 0.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	switch (m_status)
	{
	case Kart::e_basic:
		basic_wheel->Render();
		break;
	case Kart::e_pikachu:
		pikachu_wheel->Render();
		break;
	case Kart::e_eevee:
		eevee_wheel->Render();
		break;
	case Kart::e_mew:
		mew_wheel->Render();
		break;
	case Kart::e_squirtle:
		squirtle_wheel->Render();
		break;
	default:
		basic_wheel->Render();
		break;
	}
	model.PopMatrix(); // 2

	// Back left wheel
	model.PushMatrix(); // 2
	model.Translate(backLeftPos.x, backLeftPos.y, backLeftPos.z);
	model.Rotate(static_cast<float>(wheelRotation), 1.0f, 0.0f, 0.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	switch (m_status)
	{
	case Kart::e_basic:
		basic_wheel->Render();
		break;
	case Kart::e_pikachu:
		pikachu_wheel->Render();
		break;
	case Kart::e_eevee:
		eevee_wheel->Render();
		break;
	case Kart::e_mew:
		mew_wheel->Render();
		break;
	case Kart::e_squirtle:
		squirtle_wheel->Render();
		break;
	default:
		basic_wheel->Render();
		break;
	}
	model.PopMatrix(); // 2

	// Back right wheel
	model.PushMatrix(); // 2
	model.Translate(backRightPos.x, backRightPos.y, backRightPos.z);
	model.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
	model.Rotate(static_cast<float>(-wheelRotation), 1.0f, 0.0f, 0.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	switch (m_status)
	{
	case Kart::e_basic:
		basic_wheel->Render();
		break;
	case Kart::e_pikachu:
		pikachu_wheel->Render();
		break;
	case Kart::e_eevee:
		eevee_wheel->Render();
		break;
	case Kart::e_mew:
		mew_wheel->Render();
		break;
	case Kart::e_squirtle:
		squirtle_wheel->Render();
		break;
	default:
		basic_wheel->Render();
		break;
	}
	model.PopMatrix(); // 2

	// Steering wheel
	model.PushMatrix(); // 2
	model.Translate(steeringPos.x, steeringPos.y, steeringPos.z);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	steeringWheel->Render();
	model.PopMatrix(); // 2
	model.PopMatrix(); // 1
}

// Reset kart position, rotation, velocity and gear
void Kart::reset()
{
	pos = velocity = Vector3();
	yaw = pitch = roll = speed = turnForce = 0.0f;
	isDriveGear = true;
	gearShiftDelay = 0.0;
}

// Get kart position in world space
const Vector3& Kart::getPos() const
{
	return pos;
}
// Get kart velocity
const Vector3& Kart::getVel() const
{
	return velocity;
}
// Get kart rotation
double Kart::getYaw() const
{
	return yaw;
}

// Generate HUD text for speed
std::string Kart::getSpeedText() const
{
	std::stringstream convert;
	convert.precision(1);

	convert << std::fixed << speed;
	return convert.str() + "mph";
}
// Generate HUD text for drive gear
std::string Kart::getGear() const
{
	if (gearShiftDelay <= 0.0) // Not neutral
	{
		if (isDriveGear) // Driving
			return "D";
		else // Reverse
			return "R";
	}
	else // Neutral
		return "N";
}

void Kart::changeStatus()
{
	int m_random;
	m_random = (1 + rand() % 5);
	std::cout << m_random << std::endl;
	player_used = false;
	switch (m_random)
	{
	case 1:
	
		m_status = e_basic;
		
		break;
	case 2:
		
		m_status = e_pikachu;
		
		break;
	case 3:
	
		m_status = e_eevee;
		
		break;
	case 4:
		m_status = e_mew;
		break;
	case 5:
		m_status = e_squirtle;
	
		break;
	default:
		break;
	}
}

// Set kart velocity to 0
void Kart::stop()
{
	speed = 0.0;
}

void Kart::moveObject(const Vector3& displacement)
{
	pos += displacement;
	setCollisionPosition(Vector3(pos.x, pos.y + 2.0f, pos.z));
}


bool Kart::getInvert2()
{
	return player2_invert_control;
}

bool Kart::getInvert1()
{
	return player1_invert_control;
}

bool Kart::getSlow2()
{
	return player2_slow;
}

bool Kart::getSlow1()
{
	return player1_slow;
}

bool Kart::getStun1()
{
	return player1_stun;
}

bool Kart::getStun2()
{
	return player2_stun;
}

Kart::status Kart::getStatus() const
{
	return m_status;
}

bool::Kart::get_used() const
{
	return player_used;
}


Kart::buff Kart::getBuff() const
{
	return m_buff;
}


