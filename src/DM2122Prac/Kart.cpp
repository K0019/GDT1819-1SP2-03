#include "Kart.h"

// Constructor
Kart::Kart(Mesh* body, Mesh* wheel, Mesh* steeringWheel,
	const Vector3& wheelFrontLeftPos, const Vector3& wheelFrontRightPos,
	const Vector3& wheelBackLeftPos, const Vector3& wheelBackRightPos,
	const Vector3& steeringWheelPos, unsigned int uSpotLight)
	: pos(Vector3())
	, velocity(FRONT)
	, FRONT(Vector3(0, 0, 1))
	, UP(Vector3(0, 1, 0))
	, yaw(0.0)// this is the yaw angle
	, pitch(0.0)// this is pitch angle
	, roll(0.0)// this is row angle
	, speed(0.0)
	, turnForce(0.0)
	, wheelRotation(0.0)
	, isDriveGear(true)
	, jumping(false)
	, verticalVelocity(0.0f)
	, upOffset(0.0f)
	, driftOffsetDegree(0.0)
	, gearShiftDelay(0.0)
	, body(body)
	, wheel(wheel)
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
}

// Destructor
Kart::~Kart()
{
	// Delete meshes
	delete body;
	delete wheel;
	delete steeringWheel;
}

// Move the kart and handle input
void Kart::update(GLFWwindow* window, double deltaTime, unsigned int uSpotLight)
{
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
	if (isPressed(window, GLFW_KEY_LEFT_SHIFT))
	{
		if (!jumping)
		{
			jumping = true;
			verticalVelocity = 8.0f;
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

	if (jumping)
	{
		verticalVelocity -= 40.0f * static_cast<float>(deltaTime);
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
	upOffset += verticalVelocity * static_cast<float>(deltaTime);
	if (upOffset < 0.0f)
	{
		upOffset = 0.0f;
		jumping = false;
	}
	
	// Spotlight
	MS model;
	model.LoadIdentity();

	// Translation
	model.PushMatrix(); // 1
	model.Translate(pos.x, pos.y + upOffset, pos.z);
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
}

// Render the kart
void Kart::render(unsigned int uMatrixMVS) const
{
	MS model;
	model.LoadIdentity();

	// Body
	model.PushMatrix(); // 1
	model.Translate(pos.x, pos.y + 0.4f + upOffset, pos.z);
	//model.Rotate(roll, static_cast<float>(sin(yaw)), static_cast<float>(sin(pitch)), static_cast<float>(cos(yaw)));
	model.Rotate(static_cast<float>(yaw), 0.0f, 1.0f, 0.0f);
	model.Rotate(static_cast<float>(pitch), 1.0f, 0.0f, 0.0f);
	model.Rotate(static_cast<float>(roll), 0.0f, 0.0f, 1.0f);
	model.Scale(0.5f, 0.5f, 0.5f);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	body->Render();

	// Front left wheel
	model.PushMatrix(); // 2
	model.Translate(frontLeftPos.x - 0.5f, frontLeftPos.y, frontLeftPos.z);	
	model.Rotate(static_cast<float>(turnForce) * 25.0f, 0.0f, 1.0f, 0.0f);
	model.Translate(0.5f, 0, 0);
	model.Rotate(static_cast<float>(wheelRotation), 1.0f, 0.0f, 0.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	wheel->Render();
	model.PopMatrix(); // 2

	// Front right wheel
	model.PushMatrix(); // 2
	model.Translate(frontRightPos.x+0.5, frontRightPos.y, frontRightPos.z);
	model.Rotate(180.0f + static_cast<float>(turnForce) * 25.0f, 0.0f, 1.0f, 0.0f);
	model.Translate(0.5f, 0, 0);
	model.Rotate(static_cast<float>(-wheelRotation), 1.0f, 0.0f, 0.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	wheel->Render();
	model.PopMatrix(); // 2

	// Back left wheel
	model.PushMatrix(); // 2
	model.Translate(backLeftPos.x, backLeftPos.y, backLeftPos.z);
	model.Rotate(static_cast<float>(wheelRotation), 1.0f, 0.0f, 0.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	wheel->Render();
	model.PopMatrix(); // 2

	// Back right wheel
	model.PushMatrix(); // 2
	model.Translate(backRightPos.x, backRightPos.y, backRightPos.z);
	model.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
	model.Rotate(static_cast<float>(-wheelRotation), 1.0f, 0.0f, 0.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	wheel->Render();
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
	/*std::cout << "The Yaw is " << yaw << std::endl;
	std::cout << "The vector  is " << FRONT.x << " "<< FRONT.y << " " <<FRONT.z<< std::endl;
	std::cout << "The  velocity vector  is " << velocity.x << " " << velocity.y << " " << velocity.z << std::endl;
	std::cout << "The  direction velocity vector  is " << velocitydir.x << " " << velocitydir.y << " " << velocitydir.z << std::endl;
	std::cout << "The Yaw  pitch row is " << yaw << "  " << pitch << "  " << roll << std ::endl;*/
	return yaw;
}

Vector3 Kart::getCamDir() const
{
	return velocity;
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
// Set kart velocity to 0
void Kart::stop()
{
	speed = 0.0;
} 

void Kart::GetYawPitchRoll(Vector3 forward, Vector3 up, double& yaw, double& pitch, double& roll)
{
	Mtx44 forwardLeftHanded, upLeftHanded, inversion;
	forwardLeftHanded.SetToScale(forward.x, forward.y, forward.z);
	upLeftHanded.SetToScale(up.x, up.y, up.z);
	inversion.SetToScale(-1.0f, 1.0f, 1.0f);

	forwardLeftHanded = inversion * forwardLeftHanded * inversion;
	upLeftHanded = inversion * upLeftHanded * inversion;

	forward = Vector3(forwardLeftHanded.a[0], forwardLeftHanded.a[5], forwardLeftHanded.a[10]).Normalize();
	up = Vector3(upLeftHanded.a[0], upLeftHanded.a[5], upLeftHanded.a[10]).Normalize();

	yaw = Math::RadianToDegree(atan2f(forward.x, forward.z));
	pitch = Math::RadianToDegree(asinf(-forward.y));

	Vector3 orthoX = Vector3(0.0f, 1.0f, 0.0f).Cross(forward).Normalize();
	Vector3 orthoY = forward.Cross(orthoX).Normalize();

	float cosRoll = orthoY.Dot(up);

	int largest = (orthoX.x > orthoX.y ? 0 : 1);
	if (orthoX.z > (largest == 0 ? orthoX.x : orthoX.y))
		largest = 2;

	float sinRoll;
	switch (largest)
	{
	case 0: // x is largest
		sinRoll = (orthoY.x * cosRoll - up.x) / orthoX.x;
		break;
	case 1:
		sinRoll = (orthoY.y * cosRoll - up.y) / orthoX.y;
		break;
	case 2:
		sinRoll = (orthoY.z * cosRoll - up.z) / orthoX.z;
		break;
	}

	if (sinRoll >= 0)
	{
		roll = Math::RadianToDegree(acosf(cosRoll));
	}
	else
	{
		roll = 360.0f - Math::RadianToDegree(acosf(cosRoll));
	}

	/* Original Code by: Jonathan */
	
	//pitch = Math::RadianToDegree(asin(-forward.y));
	//double cosPitch = sqrt(1 - forward.y*forward.y);

	////Check if we are looking straight up or down
	//if (cosPitch == 0 || fabs(forward.y) >= 1)
	//{
	//	if (pitch > 0)
	//	{
	//		yaw = 0;
	//		roll = Math::RadianToDegree(atan2(-up.x, -up.z)) + 180 ;
	//	}
	//	else
	//	{
	//		yaw = 0;
	//		roll = Math::RadianToDegree(-atan2(up.x, up.z)) + 180;
	//	}
	//}
	//else
	//{

	//	double cosYaw = forward.z / cosPitch;
	//	double sinYaw = forward.x / cosPitch;
	//	yaw = Math::RadianToDegree(atan2(sinYaw, cosYaw));

	//	double cosRoll = up.y / cosPitch;
	//	double sinRoll;
	//	if (fabs(cosYaw) < fabs(sinYaw))
	//	{
	//		sinRoll = -(up.z + forward.y*cosRoll*cosYaw) / sinYaw;
	//	}
	//	else
	//	{
	//		sinRoll = (up.x + forward.y*cosRoll*sinYaw) / cosYaw;
	//	}
	//	roll = Math::RadianToDegree(atan2(sinRoll, cosRoll));
	//}


	////Keep all angles in [0, 360]
	//if (yaw < 0)
	//	yaw += 360;
	//else if (yaw >= 360)
	//	yaw -= 360;

	//if (pitch < 0)
	//	pitch += 360;
	//else if (pitch >= 360)
	//	pitch -= 360;

	//if (roll < 0)
	//	roll += 360;
	//else if (roll >= 360)
	//	roll -= 360;
}
