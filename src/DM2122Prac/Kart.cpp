#include "Kart.h"

// Constructor
Kart::Kart(Mesh* basic, Mesh* pikachu, Mesh* eevee, Mesh*mew, Mesh*squirtle,
	Mesh* basic_wheel, Mesh* pikachu_wheel, Mesh* eevee_wheel, Mesh* mew_wheel, Mesh* squirtle_wheel, Mesh* steeringWheel,
	const Vector3& wheelFrontLeftPos, const Vector3& wheelFrontRightPos,
	const Vector3& wheelBackLeftPos, const Vector3& wheelBackRightPos,
	const Vector3& steeringWheelPos, unsigned int uSpotLight, const OBB& obb)
	: MovingPhysicsObject(obb)
	, pos(Vector3())
	, velocity(Vector3())
	, yaw(0.0)
	, pitch(0.0)
	, roll(0.0)
	, speed(0.0)
	, turnForce(0.0)
	, wheelRotation(0.0)
	, isDriveGear(true)
	, gearShiftDelay(0.0)
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
	for (int i = 0; i < 2; ++i)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i, 12, &spotLights[i].position.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 12, 4, &spotLights[i].constant);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 16, 4, &spotLights[i].linear);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 20, 4, &spotLights[i].quadratic);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 32, 12, &spotLights[i].ambient.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 48, 12, &spotLights[i].diffuse.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 64, 12, &spotLights[i].specular.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 80, 12, &spotLights[i].direction.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 92, 4, &spotLights[i].cosInner);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 96, 4, &spotLights[i].cosOuter);
	}

	m_status = e_basic;
	
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
void Kart::update(GLFWwindow* window, double deltaTime, unsigned int uSpotLight)
{
	if (isPressed(window, GLFW_KEY_P))
	{
		m_status = e_basic;
	}
	if (isPressed(window, GLFW_KEY_O))
	{
		m_status = e_pikachu;
	}
	if (isPressed(window, GLFW_KEY_L))
	{
		m_status = e_eevee;
	}
	if (isPressed(window, GLFW_KEY_K))
	{
		m_status = e_mew;
	}
	if (isPressed(window, GLFW_KEY_I))
	{
		m_status = e_squirtle;
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
	for (int i = 0; i < 2; ++i)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i, 12, &spotLights[i].position.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 12, 4, &spotLights[i].constant);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 16, 4, &spotLights[i].linear);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 20, 4, &spotLights[i].quadratic);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 32, 12, &spotLights[i].ambient.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 48, 12, &spotLights[i].diffuse.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 64, 12, &spotLights[i].specular.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 80, 12, &spotLights[i].direction.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 92, 4, &spotLights[i].cosInner);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 96, 4, &spotLights[i].cosOuter);
	}

	// Update OBB
	setCollisionPosition(Vector3(pos.x, pos.y + 2.0f, pos.z));
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
	model.Translate(frontRightPos.x+0.5f, frontRightPos.y, frontRightPos.z);
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
	std::cout << "Change" << std::endl;
}

// Set kart velocity to 0
void Kart::stop()
{
	speed = 0.0;
}