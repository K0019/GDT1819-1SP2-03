#include "Scene2.h"
#include "GL\glew.h"
#include <map>

/* Initialize static member variables | Resolve LNK2001 */
double Scene2::deltaTime = 0.0, Scene2::enterBounce = 0.0;
int Scene2::width = PROGRAM_WIDTH, Scene2::height = PROGRAM_HEIGHT;
unsigned int Scene2::uMatrixMVS = NULL, Scene2::uMatrixP = NULL, Scene2::uColorData = NULL, Scene2::uSpotLight;
unsigned Scene2::m_programID = 0;



// Function called every update cycle, checks for keyboard input
void Scene2::processInput(GLFWwindow* window)
{
	// Check window close
	if (isPressed(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Update the player and kart(if any)
	player->update(window, deltaTime);

	// Attach/Detach kart to/from player
	if (isPressed(window, GLFW_KEY_E))
	{
		if (enterBounce <= 0.0)
		{
			enterBounce = 0.250;

			if (player->hasKart())
			{
				player->attachKart(nullptr);
			}
			else
			{
				player->attachKart(kart);
			}
		}
		else
		{
			enterBounce -= deltaTime;
		}
	}
	else
	{
		enterBounce = 0.0;
	}

	// Reset
	if (isPressed(window, GLFW_KEY_F))
	{
		player->reset();
		kart->reset();
	}

	// Misc.
	if (isPressed(window, GLFW_KEY_F1)) // Enable cull face
	{
		glEnable(GL_CULL_FACE);
		isCullFace = true;
	}
	else if (isPressed(window, GLFW_KEY_F2)) // Disable cull face
	{
		glDisable(GL_CULL_FACE);
		isCullFace = false;
	}
	if (isPressed(window, GLFW_KEY_F3)) // Disable wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (isPressed(window, GLFW_KEY_F4)) // Enable wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Scene2::resize(int width, int height)
{
	Scene2::width = width;
	Scene2::height = height;

	Mtx44 projection;
	projection.SetToPerspective(41.25 * static_cast<double>(width) / static_cast<double>(height), static_cast<double>(width) / static_cast<double>(height), 0.1, 100.0);
	updateUBO(uMatrixP, 0, sizeof(Mtx44), projection.a);
}

// Constructor
Scene2::Scene2()
{
}

// Destructor
Scene2::~Scene2()
{
}

// Initialize
void Scene2::Init()
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
	glGenBuffers(1, &uSpotLight);

	// Allocate memory for each UBO
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44) + 16 + 80 * NO_OF_POINTLIGHTS, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uMatrixMVS);

	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixP);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Mtx44), NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, uMatrixP);

	glBindBuffer(GL_UNIFORM_BUFFER, uColorData);
	glBufferData(GL_UNIFORM_BUFFER, 52, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, uColorData);

	glBindBuffer(GL_UNIFORM_BUFFER, uSpotLight);
	glBufferData(GL_UNIFORM_BUFFER, 448, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, uSpotLight);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Initialize model, view, projection matrices within shaders
	Mtx44 MVP[3];
	MVP[0].SetToIdentity();
	MVP[1].SetToLookAt(player->getCam().pos.x, player->getCam().pos.y, player->getCam().pos.z,
		player->getCam().pos.x + player->getCam().front.x, player->getCam().pos.y + player->getCam().front.y, player->getCam().pos.z + player->getCam().front.z,
		player->getCam().up.x, player->getCam().up.y, player->getCam().up.z);
	MVP[2].SetToPerspective(41.25 * static_cast<double>(width) / static_cast<double>(height), static_cast<double>(width) / static_cast<double>(height), 0.1, 100.0);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 2 * sizeof(Mtx44), MVP[0].a);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixP);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), MVP[2].a);

	// Initialize directional light
	Vector3 sunDir = Vector3(0.4f, -0.35f, 0.85f).Normalize();
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44), 12, &sunDir.x);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	SpotLight spotLight;
	spotLight.ambient = spotLight.diffuse = spotLight.specular = Vector3();
	glBindBuffer(GL_UNIFORM_BUFFER, uSpotLight);
	for (int i = 2; i < 4; ++i)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i, 12, &spotLight.position.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 12, 4, &spotLight.constant);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 16, 4, &spotLight.linear);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 20, 4, &spotLight.quadratic);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 32, 12, &spotLight.ambient.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 48, 12, &spotLight.diffuse.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 64, 12, &spotLight.specular.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 80, 12, &spotLight.direction.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 92, 4, &spotLight.cosInner);
		glBufferSubData(GL_UNIFORM_BUFFER, 112 * i + 96, 4, &spotLight.cosOuter);
	}

	// Allocate memory for each object required in Scene2
	axes = new Base3DPoly(MeshBuilder::GenerateAxes());
	floor = MeshBuilder::GenerateGrass("Image//grass.tga", 200.0f, 20);
	hotbar = new Hotbar(MeshBuilder::GenerateXYPlane("Image//slotBox.tga", 2.0f, 1, type::SHADER_TEXT),
						MeshBuilder::GenerateXYPlane("Image//selectionBox.tga", 2.125f, 1, type::SHADER_TEXT),
						MeshBuilder::GenerateSprite(8, 8, "Image//PlaceableObjects//spritesheet.tga"),
						5);
	// Source: http://www.custommapmakers.org/skyboxes.php
	// Row 5, Column 1
	skybox = new Base3DPoly(MeshBuilder::GenerateSkybox(
		"Image//Skybox//back.tga",
		"Image//Skybox//front.tga",
		"Image//Skybox//top.tga",
		"Image//Skybox//bottom.tga",
		"Image//Skybox//left.tga",
		"Image//Skybox//right.tga"
		));
	player = new Player(uMatrixMVS, uColorData);
	kart = new Kart(MeshBuilder::GenerateOBJ("OBJ//basic_kart.obj", "Image//basic_texture.tga", type::SHADER_3),
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
					OBB(Vector3(0.0f, 2.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), 2.0f, 2.0f, 2.0f), Vector3(), 0); // Steering wheel
	ModGate::detector.registerKart(kart);
	handleLap = new HandleLap(&objectList, { kart });

	text = MeshBuilder::GenerateText(16, 16, "Image//calibri.tga");
	placeObjHandler = new PlaceObjectHandler(&objectList, player, hotbar);
	

	// Enable culling and depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	isCullFace = true;
}

// Update the scene
void Scene2::Update(double dt, GLFWwindow* programID)
{
	// Update deltaTime
	deltaTime = dt;
	// Process keyboard input
	processInput(programID);
	hotbar->update(programID);
	placeObjHandler->update(programID, dt);

	Physics::physicsEngine.update();
	ModGate::detector.update();
	kart->updateOpenGL(uSpotLight);
	player->updateCamera(programID);

	handleLap->update();

	// Update shaders with new view matrix
	Mtx44 view;
	view.SetToLookAt(player->getCam().pos.x, player->getCam().pos.y, player->getCam().pos.z,
		player->getCam().pos.x + player->getCam().front.x, player->getCam().pos.y + player->getCam().front.y, player->getCam().pos.z + player->getCam().front.z,
		player->getCam().up.x, player->getCam().up.y, player->getCam().up.z);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Mtx44), sizeof(Mtx44), view.a);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

// Render the scene
void Scene2::Render()
{
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uMatrixMVS);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, uMatrixP);

	Mtx44 offset;
	offset.SetToIdentity();
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), offset.a);

	// Clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox->draw(); // Draw skybox first

	axes->draw(); // Draw the axes
	floor->render(player->getCam().pos, uMatrixMVS); // Draw the floor
	kart->render(uMatrixMVS); // Draw the kart
	player->render(); // Draw player (if applicable)

	// Draw each object in the world
	objectList.renderObjects(uMatrixMVS);

	// Draw selection if kart is not attached to player
	if (!player->hasKart())
		placeObjHandler->drawSelection(uMatrixMVS);

	// Prepare projection matrix for HUD rendering
	Mtx44 projection;
	projection.SetToOrtho(-10.0 * static_cast<double>(width) / static_cast<double>(height), 10.0 * static_cast<double>(width) / static_cast<double>(height), 0.0, 20.0, -0.1, 0.1);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixP);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), projection.a);

	// Render text
	text->PrintTextForward("FPS:" + calculateFPS(), uMatrixMVS, (-10.0f * static_cast<float>(width) / static_cast<float>(height)) / 2.0f, 1.2f, 2.0f);
	text->PrintTextBackward(kart->getSpeedText(), uMatrixMVS, 9.0f * static_cast<float>(width) / static_cast<float>(height), 2.5f, 1.0f);
	text->PrintTextBackward("Gear:" + kart->getGear(), uMatrixMVS, 9.0f * static_cast<float>(width) / static_cast<float>(height), 3.5f, 1.0f);

	// Render intersection coordinates
	Vector3 intersection;
	if (linePlaneIntersect(player->getCam().pos, player->getCam().front, Vector3(), Vector3(0.0f, 1.0f, 0.0f), intersection))
	{
		std::stringstream convert;
		convert.precision(1);
		convert << std::fixed << intersection.x << ',' << intersection.y << ',' << intersection.z;
		text->PrintTextForward(convert.str(), uMatrixMVS, -10.0f * static_cast<float>(width) / static_cast<float>(height), 16.0f, 1.0f);
	}

	// Render player coordinates
	drawCoordinates();

	// Render the hotbar
	hotbar->print(uMatrixMVS, uMatrixP, width, height);

	// Reset projection
	projection.SetToPerspective(41.25 * static_cast<double>(width) / static_cast<double>(height), static_cast<double>(width) / static_cast<double>(height), 0.1, 100.0);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixP);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), projection.a);
}

// Cleanup
void Scene2::Exit()
{
	// Free allocated memory for all the meshes
	delete axes;
	delete floor;
	delete hotbar;
	delete skybox;
	delete player;
	ModGate::detector.removeKart(kart);
	delete handleLap;
	delete kart;
	delete text;
	delete placeObjHandler;

	// Free memory allocated for UBOs
	glDeleteBuffers(1, &uMatrixMVS);
	glDeleteBuffers(1, &uMatrixP);
	glDeleteBuffers(1, &uColorData);
	glDeleteBuffers(1, &uSpotLight);
}

void Scene2::saveMap()
{
	objectList.saveObject();
}

void Scene2::loadMap()
{
	placeObjHandler->Loadmap();
}

void Scene2::clearMap()
{
	objectList.deleteAll();
}

// Generate HUD text for FPS
std::string Scene2::calculateFPS() const
{
	std::stringstream convert;
	convert.precision(2);
	
	convert << std::fixed << 1.0 / deltaTime;
	return convert.str();
}

// Render coordinate text to screen
void Scene2::drawCoordinates() const
{
	std::stringstream convertX, convertY, convertZ;

	convertX.precision(1);
	convertX << std::fixed << player->getCam().pos.x;
	text->PrintTextForward("x:" + convertX.str(), uMatrixMVS, -10.0f * static_cast<float>(width) / static_cast<float>(height), 19.0f, 1.0f);

	convertY.precision(1);
	convertY << std::fixed << player->getCam().pos.y;
	text->PrintTextForward("y:" + convertY.str(), uMatrixMVS, -10.0f * static_cast<float>(width) / static_cast<float>(height), 18.0f, 1.0f);

	convertZ.precision(1);
	convertZ << std::fixed << player->getCam().pos.z;
	text->PrintTextForward("z:" + convertZ.str(), uMatrixMVS, -10.0f * static_cast<float>(width) / static_cast<float>(height), 17.0f, 1.0f);

	//text->PrintTextForward(std::to_string(Physics::physicsEngine.testCollision()), uMatrixMVS, -10.0f * static_cast<float>(width) / static_cast<float>(height), 15, 1);
}