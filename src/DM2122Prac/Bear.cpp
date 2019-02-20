#include "Bear.h"

// Constructor
Bear::Bear(unsigned int uMatrixMVS, unsigned int uColorData)
	: uMatrixMVS(uMatrixMVS) // UBO handle for model and view matrices
	, uColorData(uColorData) // UBO handle for color
	, state(NONE) // Initialize animation state to none
{
	// Generate all the meshes required
	mesh[meshType::SPHERE] = MeshBuilder::GenerateSphere(1.0f, uColorData, false);
	mesh[meshType::CYLINDER] = MeshBuilder::GenerateCylinder(1.0f, uColorData);
	mesh[meshType::HEMISPHERE] = MeshBuilder::GenerateHemisphere(1.0f, uColorData);
	mesh[meshType::QUARTERSPHERE] = MeshBuilder::GenerateQuartersphere(1.0f, uColorData);
	mesh[meshType::SPLICEDSPHERE6] = MeshBuilder::GenerateSplicedSphere(1.0f, 180 / (SPHERE_VERTICAL_FIDELITY) / 5, uColorData);
	mesh[meshType::SQUARE] = MeshBuilder::GenerateSquare(uColorData);
	mesh[meshType::TILTED_SPHERICAL_FRUSTUM] = MeshBuilder::GenerateTiltedSphericalFrustum(uColorData);
	mesh[meshType::CONE] = MeshBuilder::GenerateCone(1.0f, uColorData);
	mesh[meshType::TRIANGLE] = MeshBuilder::GenerateTriangle(uColorData);
	mesh[meshType::TORUS] = MeshBuilder::GenerateTorus(0.9f, 1.0f, uColorData);

	// Create and start timer
	timer = new StopWatch();
	timer->startTimer();
	totalTime = timer->getElapsedTime();
}

// Destructor
Bear::~Bear()
{
	// Free up memory used for meshes
	for (int i = 0; i < meshType::NO_OF_MESHES; ++i)
	{
		delete mesh[i];
	}
	// Free up memory for timer
	delete timer;
}

// UNUSED - Update function; direction is for the bear's walking direction
void Bear::update(const Vector3& direction)
{
	// Empty for this assignment
	// TO BE REMOVED
}

// UNUSED - Change animation state
bool Bear::changeState(animationState state)
{
	// Determine if camera should reset its position, only if animation changed to WALKING
	bool changePos = state == WALKING && this->state != WALKING;
	
	// If new state does not equal current state
	if (this->state != state)
	{
		// Update state and reset timer
		this->state = state;
		delete timer;
		timer = new StopWatch();
		timer->startTimer();
		totalTime = timer->getElapsedTime();
	}

	// Return changePos
	return changePos;

	// TO BE REMOVED
}

// Get current animation state
Bear::animationState Bear::getState() const
{
	return state;
}

// Render the bear
void Bear::draw(const Vector3& offset, float yaw) const
{
	/* Numbers after push and pop matrix functions indicate which level of matrix within matrixStack is being pushed and popped */

	// Set OpenGL state (UBO)
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);

	// Start matrixStack with input offset and position offset
	MS matrixStack;
	matrixStack.PushMatrix(); // 0
	matrixStack.LoadIdentity();
	matrixStack.Translate(offset.x, offset.y, offset.z);
	matrixStack.Rotate(yaw, 0.0f, 1.0f, 0.0f);

	// Body main (Spliced sphere)
	matrixStack.PushMatrix(); // 1
	matrixStack.Translate(0.f, 1.f, 0.f);
	matrixStack.Scale(0.3f, 0.3f, 0.3f);
	matrixStack.Scale(1.3f, 2.4f, 1.3f);
	matrixStack.Translate(0.0f, 0.15f, 0.0f);
	renderMesh(&matrixStack, mesh[meshType::SPLICEDSPHERE6]);
	matrixStack.PopMatrix(); // 1
							 // Body bottom (Hemisphere)
	matrixStack.PushMatrix(); // 1
	matrixStack.Translate(0.f, 1.0f, 0.0f);
	matrixStack.Scale(0.3f, 0.3f, 0.3f);

	matrixStack.PushMatrix(); // 2
	matrixStack.Translate(0.0f, -0.05f, 0.0f);
	matrixStack.Rotate(180, 1.0f, 0.0f, 0.0f);
	matrixStack.Scale(0.9f, 0.8f, 0.9f);
	renderMesh(&matrixStack, mesh[meshType::HEMISPHERE]);
	matrixStack.PopMatrix(); // 2

							 // Legs
	switch (state)
	{
	case NONE:
	case WAVING:
	case PLAYING:
		drawLeg(matrixStack, false, Mtx44().SetToRotation(95.0f, 1.0f, 0.0f, 0.0f)); // Right leg
		drawLeg(matrixStack, true, Mtx44().SetToRotation(95.0f, 1.0f, 0.0f, 0.0f)); // Left leg
		break;
	case WALKING:
		drawLeg(matrixStack, false, Mtx44().SetToRotation(90.0f, 1.0f, 0.0f, 0.0f)); // Right leg
		drawLeg(matrixStack, true, Mtx44().SetToRotation(90.0f, 1.0f, 0.0f, 0.0f)); // Left leg
		break;
	}

	// Arms
	drawArm(matrixStack, false, Mtx44().SetToRotation(15.0f, 0.0f, 0.0f, 1.0f)); // Right arm
	switch (state) // Arm waving
	{
	case NONE:
	case WALKING:
	case PLAYING:
		drawArm(matrixStack, true, Mtx44().SetToRotation(-15.0f, 0.0f, 0.0f, 1.0f)); // Left arm
		break;
	case WAVING:
		drawArm(matrixStack, true, Mtx44().SetToRotation(static_cast<float>(20 * sin(totalTime * 5)) + -115.0f, 0.0f, 0.0f, 1.0f)); // Right arm
		break;
	}

	// Scarf
	matrixStack.PushMatrix(); // 2
	matrixStack.Translate(0.0f, 2.9f, 0.0f);
	drawScarf(matrixStack);

	// Head
	matrixStack.PushMatrix(); // 3
	if (state == PLAYING) // Rotate head if playing
	{
		matrixStack.Rotate(-60.0f, 0.0f, 1.0f, 0.0f);
		matrixStack.Rotate(static_cast<float>(-15.0 + std::abs(5.0 * cos(totalTime * 6.0))), 1.0f, 0.0f, 0.0f);
	}
	matrixStack.Translate(0.0f, 0.9f, 0.0f);
	drawHead(matrixStack);

	// Clear model matrix in shader
	matrixStack.PopMatrix(); // 3
	matrixStack.PopMatrix(); // 2
	matrixStack.PopMatrix(); // 1
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), matrixStack.Top().a);

	// Unbind UBO state
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

// Draw the leg
void Bear::drawLeg(MS& matrixStack, bool left, const Mtx44& legRotation) const
{
	matrixStack.PushMatrix(); // 0
	if (state != WALKING) // If not walking, rotate legs slightly
	{
		matrixStack.Rotate(-5.0f, 1.0f, 0.0f, 0.0f);
	}

	// Leg connection (Sphere)
	matrixStack.PushMatrix(); // 1
	if (left) // Translate to proper position
	{
		matrixStack.Translate(-0.7f, -0.4f, 0.0f);
	}
	else
	{
		matrixStack.Translate(0.7f, -0.4f, 0.0f);
	}
	if (state == WALKING) // If walking, rotate legs progressively
	{
		if (left)
		{
			matrixStack.Rotate(20 * static_cast<float>(sin(totalTime * 5)) - 90.0f, 1.0f, 0.0f, 0.0f);
		}
		else
		{
			matrixStack.Rotate(20 * static_cast<float>(-sin(totalTime * 5)) - 90.0f, 1.0f, 0.0f, 0.0f);
		}
	}
	matrixStack.Scale(0.63f, 0.63f, 0.63f);
	renderMesh(&matrixStack, mesh[meshType::SPHERE]);

	// Leg cylinder (Cylinder)
	matrixStack.PushMatrix(); // 2
	matrixStack.MultMatrix(legRotation);
	matrixStack.Scale(1.0f, 1.45f, 1.0f);
	matrixStack.Rotate(((left) ? (-10.0f) : (10.0f)), 0.0f, 0.0f, 1.0f);
	matrixStack.Translate(0.0f, -1.0f, 0.0f);
	renderMesh(&matrixStack, mesh[meshType::CYLINDER]);

	// Foot (Sphere)
	matrixStack.PushMatrix(); // 3
	matrixStack.Scale(1.1f, 0.7f, 1.5f);
	matrixStack.Translate(0.0f, -1.7f, -0.2f);
	renderMesh(&matrixStack, mesh[meshType::SPHERE]);
	matrixStack.PopMatrix(); // 3

	matrixStack.PopMatrix(); // 2
	matrixStack.PopMatrix(); // 1
	matrixStack.PopMatrix(); // 0
}

// Draw the arm
void Bear::drawArm(MS& matrixStack, bool left, const Mtx44& armRotation) const
{
	// Arm connection (Sphere)
	matrixStack.PushMatrix(); // 0
	matrixStack.Rotate(5.0f, 1.0f, 0.0f, 0.0f);
	matrixStack.PushMatrix(); // 1
	if (left) // Translation to proper position
	{
		matrixStack.Translate(-0.9f, 2.4f, -0.55f);
	}
	else
	{
		matrixStack.Translate(0.9f, 2.4f, -0.55f);
	}
	switch (state) // Movement
	{
	case NONE:
	case WAVING:
		break;
	case WALKING:
		if (left) // Rotate progressively
		{
			matrixStack.Rotate(30 * static_cast<float>(-sin(totalTime * 5)), 1.0f, 0.0f, 0.0f);
		}
		else
		{
			matrixStack.Rotate(30 * static_cast<float>(sin(totalTime * 5)), 1.0f, 0.0f, 0.0f);
		}
		break;
	case PLAYING:
		if (!left) // If right arm, rotate progressively
		{
			matrixStack.Rotate(15.0f, 0.0f, 1.0f, 0.0f);
			matrixStack.Rotate(55.0f + static_cast<float>(std::abs(20.0 * cos(totalTime * 6.0))), 0.0f, 0.0f, 1.0f);
		}
		break;
	}
	matrixStack.Scale(0.5f, 0.5f, 0.5f);
	renderMesh(&matrixStack, mesh[meshType::SPHERE]);

	// Arm cylinder (Cylinder)
	matrixStack.PushMatrix(); // 2
	matrixStack.MultMatrix(armRotation);
	matrixStack.Scale(1.0f, 1.4f, 1.0f);
	matrixStack.Translate(0.0f, -1.0f, 0.0f);
	renderMesh(&matrixStack, mesh[meshType::CYLINDER]);

	// Arm end (Hemisphere)
	matrixStack.PushMatrix(); // 3
	matrixStack.Translate(0.0f, -1.0f, 0.0f);
	matrixStack.Rotate(180.0f, 1.0f, 0.0f, 0.0f);
	matrixStack.Scale(1.0f, 0.8f, 1.0f);
	renderMesh(&matrixStack, mesh[meshType::HEMISPHERE]);

	// Return to original position for further rendering
	matrixStack.PopMatrix(); // 3
	matrixStack.PopMatrix(); // 2
	matrixStack.PopMatrix(); // 1
	matrixStack.PopMatrix(); // 0
}

void Bear::drawScarf(MS& matrixStack) const
{
	// Set cylinder and square color to red
	mesh[meshType::CYLINDER]->setColor({ 1.0f, 0.0f, 0.0f }, 0.4f, 0.5f, 0.05f, 16);
	mesh[meshType::SQUARE]->setColor({ 1.0f, 0.0f, 0.0f }, 0.4f, 0.5f, 0.05f, 16);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);

	// Neckband (Cylinder)
	matrixStack.PushMatrix(); // 0
	matrixStack.Translate(0.0f, 0.1f, 0.0f);
	matrixStack.PushMatrix(); // 1
	matrixStack.Scale(1.07f, 0.15f, 1.07f);
	renderMesh(&matrixStack, mesh[meshType::CYLINDER]);
	matrixStack.PopMatrix(); // 1

	// First ballband front (Square 2D)
	matrixStack.PushMatrix(); // 1
	matrixStack.Translate(0.0f, 0.0f, -1.05f);
	matrixStack.PushMatrix(); // 2
	matrixStack.Rotate(-71.0f, 1.0f, 0.0f, 0.0f);
	matrixStack.Translate(0.0f, 0.0f, -0.6f);
	matrixStack.PushMatrix(); // 3
	matrixStack.Scale(0.2f, 1.0f, 0.6f);
	renderMesh(&matrixStack, mesh[meshType::SQUARE]);
	// First ballband back (Square 2D)
	matrixStack.Rotate(180.0f, 1.0f, 0.0f, 0.0f);
	renderMesh(&matrixStack, mesh[meshType::SQUARE]);
	matrixStack.PopMatrix(); // 3
	// First ball (Sphere)
	matrixStack.PushMatrix(); // 3
	matrixStack.Translate(0.0f, 0.0f, -0.6f);
	matrixStack.Scale(0.3f, 0.3f, 0.3f);
	renderMesh(&matrixStack, mesh[meshType::SPHERE]);
	matrixStack.PopMatrix(); // 3
	matrixStack.PopMatrix(); // 2
	matrixStack.PopMatrix(); // 1

	// Second ballband front (Square 2D)
	mesh[meshType::SQUARE]->setColor({ 1.0f, 0.0f, 0.0f }, 0.4f, 0.5f, 0.05f, 16);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	matrixStack.PushMatrix(); // 1
	matrixStack.Translate(0.0f, 0.0f, -1.05f);
	matrixStack.PushMatrix(); // 2
	matrixStack.Rotate(-75.0f, 1.0f, 0.0f, 0.0f);
	matrixStack.Rotate(-20.0f, 0.0f, 1.0f, 0.0f);
	matrixStack.Rotate(-15.0f, 0.0f, 0.0f, 1.0f);
	matrixStack.Translate(0.0f, 0.0f, -0.8f);
	matrixStack.PushMatrix(); // 3
	matrixStack.Scale(0.2f, 1.0f, 0.8f);
	renderMesh(&matrixStack, mesh[meshType::SQUARE]);
	// Second ballband back (Square 2D)
	matrixStack.Rotate(180.0f, 1.0f, 0.0f, 0.0f);
	renderMesh(&matrixStack, mesh[meshType::SQUARE]);
	matrixStack.PopMatrix(); // 3
	// Second ball (Sphere)
	matrixStack.PushMatrix(); // 3
	matrixStack.Translate(0.0f, 0.0f, -0.8f);
	matrixStack.Scale(0.3f, 0.3f, 0.3f);
	renderMesh(&matrixStack, mesh[meshType::SPHERE]);
	matrixStack.PopMatrix(); // 3
	matrixStack.PopMatrix(); // 2
	matrixStack.PopMatrix(); // 1

	matrixStack.PopMatrix(); // 0

	// Reset cylinder color to white
	mesh[meshType::CYLINDER]->setColor({ 1.0f, 1.0f, 1.0f }, 0.4f, 0.5f, 0.05f, 16);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
}

// Draw the head and hat
void Bear::drawHead(MS& matrixStack) const
{
	// Head (Sphere)
	matrixStack.PushMatrix(); // 0
	matrixStack.Scale(1.1f, 1.3f, 1.1f);
	renderMesh(&matrixStack, mesh[meshType::SPHERE]);
	matrixStack.PopMatrix(); // 0

	// Base Mouth (Quartersphere)
	matrixStack.PushMatrix(); // 0
	matrixStack.Translate(0.0f, -0.1f, -0.8f);
	matrixStack.PushMatrix(); // 1
	matrixStack.Rotate(180.0f, 1.0f, 0.0f, 0.0f);
	matrixStack.Scale(0.6f, 0.5f, 0.8f);
	renderMesh(&matrixStack, mesh[meshType::QUARTERSPHERE]);
	matrixStack.PopMatrix(); // 1
	// Top Mouth (Quartersphere)
	matrixStack.PushMatrix(); // 1
	matrixStack.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
	matrixStack.Scale(0.6f, 0.35f, 0.8f);
	renderMesh(&matrixStack, mesh[meshType::QUARTERSPHERE]);
	matrixStack.PopMatrix(); // 1

	// Mouth horizontal (Torus)
	matrixStack.PushMatrix(); // 1
	mesh[meshType::TORUS]->setColor(118, 91, 99, 0.4f, 0.5f, 0.05f, 16);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	matrixStack.Translate(0.0f, -0.45f, -0.08f);
	matrixStack.Rotate(-35.0f, 1.0f, 0.0f, 0.0f);
	matrixStack.Scale(0.24f, 0.25f, 0.2f);
	renderMesh(&matrixStack, mesh[meshType::TORUS]);
	matrixStack.PopMatrix(); // 1
	// Mouth vertical (Torus)
	matrixStack.PushMatrix(); // 1
	matrixStack.Translate(0.475f, -0.115f, -0.39f);
	matrixStack.Rotate(90.0f, 0.0f, 0.0f, 1.0f);
	matrixStack.Scale(0.12f, 0.25f, 0.2f);
	renderMesh(&matrixStack, mesh[meshType::TORUS]);
	matrixStack.PopMatrix(); // 1

	// Left eye (Sphere)
	// Set sphere color to black and modify specular
	mesh[meshType::SPHERE]->setColor({ 0.0f, 0.0f, 0.0f }, 0.4f, 0.5f, 0.05f, 16);
	mesh[meshType::SPHERE]->setSpecular({ 0.2f, 0.2f, 0.2f }, 4);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	matrixStack.PushMatrix(); // 1
	matrixStack.Rotate(30.0f, 0.0f, 0.0f, 1.0f);
	matrixStack.Scale(0.1f, 0.1f, 0.1f);
	matrixStack.PushMatrix(); // 2
	matrixStack.Translate(0.0f, 6.3f, -1.6f);
	renderMesh(&matrixStack, mesh[meshType::SPHERE]);
	matrixStack.PopMatrix(); // 2
	matrixStack.PopMatrix(); // 1
	//Right eye (Sphere)
	matrixStack.PushMatrix(); // 1
	matrixStack.Rotate(-30.0f, 0.0f, 0.0f, 1.0f);
	matrixStack.Scale(0.1f, 0.1f, 0.1f);
	matrixStack.PushMatrix(); // 2
	matrixStack.Translate(0.0f, 6.3f, -1.6f);
	renderMesh(&matrixStack, mesh[meshType::SPHERE]);
	matrixStack.PopMatrix(); // 2
	matrixStack.PopMatrix(); // 1
	// Reset sphere color to white
	mesh[meshType::SPHERE]->setColor({ 1.0f, 1.0f, 1.0f }, 0.4f, 0.5f, 0.05f, 16);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);

	// Snout (Sphere)
	// Set sphere color to brown
	mesh[meshType::SPHERE]->setColor(118, 91, 99, 0.4f, 0.5f, 0.05f, 16);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	matrixStack.PushMatrix(); // 1
	matrixStack.Translate(0.0f, 0.07f, -0.63f);
	matrixStack.Rotate(-65.0f, 1.0f, 0.0f, 0.0f);
	matrixStack.Scale(0.35f, 0.17f, 0.25f);
	renderMesh(&matrixStack, mesh[meshType::SPHERE]);
	matrixStack.PopMatrix(); // 1
	matrixStack.PopMatrix(); // 0
	// Reset sphere color to white
	mesh[meshType::SPHERE]->setColor({ 1.0f, 1.0f, 1.0f }, 0.4f, 0.5f, 0.05f, 16);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);

	// Left Ear (Cylinder)
	matrixStack.PushMatrix(); // 0
	matrixStack.Rotate(90.0f, 1.0f, 0.0f, 0.0f);
	matrixStack.Scale(0.3f, 0.06f, 0.3f);
	matrixStack.PushMatrix(); // 1
	matrixStack.Translate(2.8f, 0.0f, -2.8f);
	renderMesh(&matrixStack, mesh[meshType::CYLINDER]);
	matrixStack.PopMatrix(); // 1
	// Right Ear (Cylinder)
	matrixStack.PushMatrix(); // 1
	matrixStack.Translate(-2.8f, 0.0f, -2.8f);
	renderMesh(&matrixStack, mesh[meshType::CYLINDER]);
	matrixStack.PopMatrix(); // 1
	matrixStack.PopMatrix(); // 0

	// Hat base (Hemisphere)
	// Set color of tilted spherical frustum, hemisphere, cone to red
	mesh[meshType::TILTED_SPHERICAL_FRUSTUM]->setColor(255, 0, 0, 0.4f, 0.5f, 0.05f, 16);
	mesh[meshType::HEMISPHERE]->setColor(255, 0, 0, 0.4f, 0.5f, 0.05f, 16);
	mesh[meshType::CONE]->setColor(255, 0, 0, 0.4f, 0.5f, 0.05f, 16);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	matrixStack.PushMatrix(); // 0
	matrixStack.Rotate(90.0f, 0.0f, 1.0f, 0.0f);
	matrixStack.Rotate(-10.0f, 1.0f, 0.0f, 0.0f);
	matrixStack.Translate(0.0f, 0.63f, -0.1f);
	matrixStack.PushMatrix(); // 1
	matrixStack.Rotate(-10.0f, 1.0f, 0.0f, 0.0f);
	matrixStack.Rotate(180.0f, 0.0f, 0.0f, 1.0f);
	matrixStack.Scale(0.915f, 0.4f, 0.915f);
	matrixStack.Translate(0.0f, -0.46f, -0.0125f);
	renderMesh(&matrixStack, mesh[meshType::HEMISPHERE]);
	matrixStack.PopMatrix(); // 1
	// Hat middle (drawHat_NoPop())
	drawHat_NoPop(matrixStack, 16); // !!\
	// Hat end (Cone)
	matrixStack.PushMatrix(); // 2
	matrixStack.Rotate(-10.0f, 1.0f, 0.0f, 0.0f);
	matrixStack.Translate(0.0f, 1.2f, -0.01f);
	matrixStack.Scale(0.9f, 1.0f, 0.9f);
	renderMesh(&matrixStack, mesh[meshType::CONE]);
	// Hat tail (Triangle 2D)
	for (int i = 0; i < 3; ++i)
	{
		matrixStack.PushMatrix(); // 3
		matrixStack.Translate(0.0f, 1.9f, 0.0f);
		matrixStack.Rotate(120.0f * i, 0.0f, 1.0f, 0.0f);
		matrixStack.Rotate(180.0f, 1.0f, 0.0f, 0.0f);
		renderMesh(&matrixStack, mesh[meshType::TRIANGLE]);
		matrixStack.PopMatrix(); // 3
	}

	// Return position to original
	matrixStack.PopMatrix(); // 2
	for (int i = 0; i < 16; ++i)
	{
		matrixStack.PopMatrix(); // !!
	}
	matrixStack.PopMatrix(); // 1
	matrixStack.PopMatrix(); // 0

	// Reset hemisphere color to white
	mesh[meshType::HEMISPHERE]->setColor(255, 255, 255, 0.4f, 0.5f, 0.05f, 16);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
}

// Draw the middle of the hat, used in drawHead; recursive
void Bear::drawHat_NoPop(MS& matrixStack, int count) const
{
	// A slice of the middle of the hat
	matrixStack.PushMatrix();
	matrixStack.Scale(0.915f, 0.915f, 0.915f);
	matrixStack.Rotate(-10.0f, 1.0f, 0.0f, 0.0f);
	matrixStack.Translate(0.0f, 0.2f, -0.015f);
	matrixStack.PushMatrix();
	matrixStack.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
	renderMesh(&matrixStack, mesh[meshType::TILTED_SPHERICAL_FRUSTUM]);
	matrixStack.PopMatrix();

	if (--count) // Call this function again if count is not 1
	{
		drawHat_NoPop(matrixStack, count);
	}
}

// Function containing process of rendering a mesh within draw
void Bear::renderMesh(MS* const matrixStack, Mesh* const mesh) const
{
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), matrixStack->Top().a);
	mesh->Render();
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
}