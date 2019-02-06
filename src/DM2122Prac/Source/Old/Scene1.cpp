#include "Scene1.h"
#include "GL\glew.h"


/* Initialize static member variables | Resolve LNK2001 */
Camera Scene1::camera = Camera();
double Scene1::deltaTime = 0.0;
unsigned int Scene1::uMatrixMVS = NULL, Scene1::uMatrixP = NULL, Scene1::uColorData = NULL;
unsigned Scene1::m_programID = 0;
Bear* Scene1::bear = nullptr;

// Checks if a key is pressed
const bool Scene1::isPressed(GLFWwindow* window, GLenum key) const
{
	return glfwGetKey(window, key) == GLFW_PRESS;
}

// Function called every update cycle, checks for keyboard input
Vector3 Scene1::processInput(GLFWwindow* window)
{
	// Check window close
	if (isPressed(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Camera position
	Vector3 direction; // Initialize to zero vector
	if (bear->getState() == Bear::animationState::WALKING) // If animation state is walking (3rd person)
	{
		// Add unit vector in respective direction if key is pressed
		if (isPressed(window, GLFW_KEY_W))
			direction += Vector3(camera.front.x, 0.0f, camera.front.z).Normalize();
		if (isPressed(window, GLFW_KEY_S))
			direction -= Vector3(camera.front.x, 0.0f, camera.front.z).Normalize();
		if (isPressed(window, GLFW_KEY_A))
			direction -= camera.front.Cross(camera.up).Normalize();
		if (isPressed(window, GLFW_KEY_D))
			direction += camera.front.Cross(camera.up).Normalize();

		// Normalize if not zero vector
		if (direction != Vector3())
			direction.Normalize();
	}
	else // If animation state is not walking (1st person)
	{
		float cameraSpeed = (float)(5.0f * deltaTime); // Camera speed

		// Modify camera according to respective direction if key is pressed
		if (isPressed(window, GLFW_KEY_W))
			camera.pos += cameraSpeed * Vector3(camera.front.x, 0.0f, camera.front.z).Normalize();
		if (isPressed(window, GLFW_KEY_S))
			camera.pos -= cameraSpeed * Vector3(camera.front.x, 0.0f, camera.front.z).Normalize();
		if (isPressed(window, GLFW_KEY_A))
			camera.pos -= camera.front.Cross(camera.up).Normalize() * cameraSpeed;
		if (isPressed(window, GLFW_KEY_D))
			camera.pos += camera.front.Cross(camera.up).Normalize() * cameraSpeed;
		if (isPressed(window, GLFW_KEY_SPACE))
			camera.pos += cameraSpeed * camera.up;
		if (isPressed(window, GLFW_KEY_LEFT_SHIFT))
			camera.pos -= cameraSpeed * camera.up;
	}

	// Lamp position
	float lampSpeed = (float)(5.0f * deltaTime); // Lamp speed
	// Move lamp in respective direction if key is pressed
	if (isPressed(window, GLFW_KEY_I))
		lamp[0]->light.position.z += lampSpeed;
	if (isPressed(window, GLFW_KEY_K))
		lamp[0]->light.position.z -= lampSpeed;
	if (isPressed(window, GLFW_KEY_J))
		lamp[0]->light.position.x += lampSpeed;
	if (isPressed(window, GLFW_KEY_L))
		lamp[0]->light.position.x -= lampSpeed;
	if (isPressed(window, GLFW_KEY_U))
		lamp[0]->light.position.y += lampSpeed;
	if (isPressed(window, GLFW_KEY_O))
		lamp[0]->light.position.y -= lampSpeed;

	// Change animation
	if (isPressed(window, GLFW_KEY_5)) // NONE
		bear->changeState(Bear::animationState::NONE);
	else if (isPressed(window, GLFW_KEY_6)) // WAVING
		bear->changeState(Bear::animationState::WAVING);
	else if (isPressed(window, GLFW_KEY_7)) // WAKING
	{
		if (bear->changeState(Bear::animationState::WALKING)) // If previously not walking, move camera to a 3rd person position
		{
			camera.setPos(bear->getPosition() - 8.0f * camera.front);
		}
	}
	else if (isPressed(window, GLFW_KEY_8)) // PLAYING
		bear->changeState(Bear::animationState::PLAYING);

	// Reset
	if (isPressed(window, GLFW_KEY_R))
	{
		// Reset bear
		delete bear;
		bear = new Bear(uMatrixMVS, uColorData);
		direction = Vector3();

		// Reset camera
		camera.init();
	}
	
	// Misc.
	if (isPressed(window, GLFW_KEY_1)) // Enable cull face
		glEnable(GL_CULL_FACE);
	else if (isPressed(window, GLFW_KEY_2)) // Disable cull face
		glDisable(GL_CULL_FACE);
	if (isPressed(window, GLFW_KEY_3)) // Disable wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (isPressed(window, GLFW_KEY_4)) // Enable wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Return direction for bear update
	return direction;
}

// Function called when mouse moves to update camera
void Scene1::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	// Avoid camera movement for first frame, to avoid large camera jump
	if (camera.firstMouse)
	{
		camera.mouseX = (float)xpos;
		camera.mouseY = (float)ypos;
		camera.firstMouse = false;
	}
	// Update internal cursor position
	float xOffset = (float)(xpos - camera.mouseX);
	float yOffset = (float)(camera.mouseY - ypos);
	camera.mouseX = (float)xpos;
	camera.mouseY = (float)ypos;

	// Multiply by sensitivity
	float sensitivity = 0.05f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	// Modify camera pitch and yaw, clamping it to avoid Gimbal Lock
	camera.yaw += xOffset;
	camera.pitch += yOffset;
	if (camera.pitch > 89.9f)
		camera.pitch = 89.9f;
	else if (camera.pitch < -89.9f)
		camera.pitch = -89.9f;

	// Update camera front
	camera.front = Vector3((float)(cos(Math::DegreeToRadian(camera.pitch)) * cos(Math::DegreeToRadian(camera.yaw))),
							(float)(sin(Math::DegreeToRadian(camera.pitch))),
							(float)(cos(Math::DegreeToRadian(camera.pitch)) * sin(Math::DegreeToRadian(camera.yaw)))).Normalize();
	// Move camera to a 3rd person position if animation state is walking
	if (bear->getState() == Bear::animationState::WALKING)
		camera.pos = bear->getPosition() - 8.0f * camera.front;
}

// Function called when window resizes to adapt render viewport
void Scene1::framebuffer_resize_callback(GLFWwindow* window, int width, int height)
{
	// Resize viewport
	glViewport(0, 0, width, height);
	// Update projection matrix
	Mtx44 projection;
	projection.SetToPerspective(55.0, (double)width / (double)height, 0.1, 100.0);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixP);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), projection.a);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

// Constructor
Scene1::Scene1()
{
}

// Destructor
Scene1::~Scene1()
{
}

// Initialize
void Scene1::Init()
{
	// Set clear color
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Use most general shader for configuration
	shader::container.useShader(type::SHADER_2);
	m_programID = shader::container.getID(type::SHADER_2);

	// Generate UBOs
	glGenBuffers(1, &uMatrixMVS);
	glGenBuffers(1, &uMatrixP);
	glGenBuffers(1, &uColorData);

	// Allocate memory for each UBO
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44) + 80 * NO_OF_POINTLIGHTS, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uMatrixMVS);

	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixP);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Mtx44), NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, uMatrixP);
	
	glBindBuffer(GL_UNIFORM_BUFFER, uColorData);
	glBufferData(GL_UNIFORM_BUFFER, 52, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, uColorData);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Initialize model, view, projection matrices within shaders
	Mtx44 MVP[3];
	MVP[0].SetToIdentity();
	MVP[1].SetToLookAt(camera.pos.x, camera.pos.y, camera.pos.z,
		camera.pos.x + camera.front.x, camera.pos.y + camera.front.y, camera.pos.z + camera.front.z,
		camera.up.x, camera.up.y, camera.up.z);
	MVP[2].SetToPerspective(55.0, 800.0 / 600.0, 0.1, 100.0);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 2 * sizeof(Mtx44), MVP[0].a);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixP);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), MVP[2].a);
	
	// Initialize each lamp
	for (int i = 0; i < NO_OF_POINTLIGHTS; ++i)
	{
		lamp[i] = new Lamp(uColorData);
		lamp[i]->light.position = Vector3(0.0f, 10.0f, -10.0f);
		lamp[i]->light.ambient = Vector3(1.0f, 1.0f, 1.0f);
		lamp[i]->light.diffuse = Vector3(1.0f, 1.0f, 1.0f);
		lamp[i]->light.specular = Vector3(1.0f, 1.0f, 1.0f);
		lamp[i]->light.constant = 0.8f;
		lamp[i]->light.linear = 0.009f;
		lamp[i]->light.quadratic = 0.0f;
	}

	// Initialize each point light within shaders
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	for (int i = 0; i < NO_OF_POINTLIGHTS; ++i)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44) + 80 * i, 12, &lamp[i]->light.position.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44) + 80 * i + 12, 4, &lamp[i]->light.constant);
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44) + 80 * i + 16, 4, &lamp[i]->light.linear);
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44) + 80 * i + 20, 4, &lamp[i]->light.quadratic);
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44) + 80 * i + 32, 12, &lamp[i]->light.ambient.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44) + 80 * i + 48, 12, &lamp[i]->light.diffuse.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44) + 80 * i + 64, 12, &lamp[i]->light.specular.x);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Allocate memory for each object required in Scene1
	bear = new Bear(uMatrixMVS, uColorData);
	axes = new Base3DPoly(MeshBuilder::GenerateAxes());
	floor = new Base3DPoly(MeshBuilder::GenerateQuad(uColorData));
	tree = new TreeWinter(uMatrixMVS, uColorData);

	// Set floor color
	floor->setColor({ 0.9f, 0.9f, 0.9f });

	// Enable culling and depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

// Update the scene
void Scene1::Update(double dt, GLFWwindow* programID)
{
	// Update deltaTime
	deltaTime = dt;
	// Get a direction which the player wants to move in
	Vector3 direction = processInput(programID);
	// Update the bear
	bear->update(direction);
	// Update the camera position
	camera.pos += direction * 2 * static_cast<float>(dt);
	// Update shaders with new view matrix
	Mtx44 view;
	view.SetToLookAt(camera.pos.x, camera.pos.y, camera.pos.z,
					camera.pos.x + camera.front.x, camera.pos.y + camera.front.y, camera.pos.z + camera.front.z,
					camera.up.x, camera.up.y, camera.up.z);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Mtx44), sizeof(Mtx44), view.a);

	// Update point light position within shaders
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44), 12, &lamp[0]->light.position.x);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

// Render the scene
void Scene1::Render()
{
	// Initialize offset and resize model matrices for trees
	Mtx44 offset, resize;
	offset.SetToIdentity();
	resize.SetToScale(8.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), offset.a);

	// Clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	axes->draw(); // Draw the axes
	floor->draw(); // Draw the floor

	// Draw the lamps
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), offset.a);
	for (int i = 0; i < NO_OF_POINTLIGHTS; ++i)
	{
		offset.SetToTranslation(lamp[i]->light.position.x, lamp[i]->light.position.y, lamp[i]->light.position.z);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), offset.a);
		lamp[i]->draw();
	}

	// Draw each tree at a different location
	offset.SetToTranslation(0.0f, 9.6f, 30.0f);
	tree->draw(offset * resize);
	offset.SetToTranslation(20.0f, 9.6f, 20.0f);
	tree->draw(offset * resize);
	offset.SetToTranslation(-20.0f, 9.6f, 20.0f);
	tree->draw(offset * resize);

	// Draw the bear
	offset.SetToTranslation(0.0f, 2.0f, 0.0f);
	bear->draw(offset);
}

// Cleanup
void Scene1::Exit()
{
	// Free allocated memory for all the meshes
	for (int i = 0; i < NO_OF_POINTLIGHTS; ++i)
	{
		delete lamp[i];
	}
	delete bear;
	delete axes;
	delete floor;
	delete tree;

	// Free memory allocated for UBOs
	glDeleteBuffers(1, &uMatrixMVS);
	glDeleteBuffers(1, &uMatrixP);
	glDeleteBuffers(1, &uColorData);
}
