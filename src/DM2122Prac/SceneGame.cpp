#include "SceneGame.h"

/* Initialize static member variables | Resolve LNK2001 */
double SceneGame::deltaTime = 0.0, SceneGame::enterBounce = 0.0;
int SceneGame::width = 800, SceneGame::height = 600;
unsigned int SceneGame::uMatrixMVS = NULL, SceneGame::uMatrixP = NULL, SceneGame::uColorData = NULL, SceneGame::uSpotLight;
unsigned SceneGame::m_programID = 0;

SceneGame::SceneGame()
{
}


SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	// Set clear color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	placing = 1;
	// Use most general shader for configuration
	shader::container.useShader(type::SHADER_2);
	m_programID = shader::container.getID(type::SHADER_2);
	music::player.init();
	music::player.playsound("Sound/test.mp3", true);
	music::player.setsoundvol(0.1f);
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
	glBufferData(GL_UNIFORM_BUFFER, 224, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, uSpotLight);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	
	// Initialize directional light
	Vector3 sunDir = Vector3(0.4f, -0.35f, 0.85f).Normalize();
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44), 12, &sunDir.x);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	// Allocate memory for each object required in SceneGame
	axes = new Base3DPoly(MeshBuilder::GenerateAxes());
	floor = MeshBuilder::GenerateGrass("Image//grass.tga", 200.0f, 20);

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
	player[0] = new c_m_Player(uSpotLight);
	player[1] = new c_m_Player(uSpotLight);
	hotbar = new Hotbar(MeshBuilder::GenerateXYPlane("Image//slotBox.tga", 2.0f, 1, type::SHADER_TEXT),
		MeshBuilder::GenerateXYPlane("Image//selectionBox.tga", 2.125f, 1, type::SHADER_TEXT),
		MeshBuilder::GenerateSprite(8, 8, "Image//PlaceableObjects//spritesheet.tga"),
		5);
	playerDummy = new Player(uMatrixMVS, uColorData);

	text = MeshBuilder::GenerateText(16, 16, "Image//calibri.tga");
	GUI[0] = MeshBuilder::GenerateXYPlane("Image//slotBox.tga", 2.0f, 1, type::SHADER_TEXT); 
	GUI[1] = MeshBuilder::GenerateXYPlane("Image//1place.tga", 4.0f, 1, type::SHADER_TEXT); 
	GUI[2] = MeshBuilder::GenerateXYPlane("Image//2place.tga", 4.0f, 1, type::SHADER_TEXT);

	GUI[3] = MeshBuilder::GenerateXYPlane("Image//nothing.tga", 2.0f, 1, type::SHADER_TEXT);
	GUI[4] = MeshBuilder::GenerateXYPlane("Image//stun.tga", 2.0f, 1, type::SHADER_TEXT);
	GUI[5] = MeshBuilder::GenerateXYPlane("Image//speed_up.tga", 2.0f, 1, type::SHADER_TEXT);
	GUI[6] = MeshBuilder::GenerateXYPlane("Image//confuse.tga", 2.0f, 1, type::SHADER_TEXT);
	GUI[7] = MeshBuilder::GenerateXYPlane("Image//slow.tga", 2.0f, 1, type::SHADER_TEXT);

	Map = new PlaceObjectHandler(&objectList, playerDummy, hotbar);
	Map->Loadmap();
	handleLap = new HandleLap(&objectList, { player[0]->getCar(), player[1]->getCar() });
	countdown = new Countdown(MeshBuilder::GenerateXYPlane("Image//three.tga", 12.0f, 6.75f, 1, type::SHADER_WINLOSE),
							  MeshBuilder::GenerateXYPlane("Image//two.tga", 12.0f, 6.75f, 1, type::SHADER_WINLOSE),
							  MeshBuilder::GenerateXYPlane("Image//one.tga", 12.0f, 6.75f, 1, type::SHADER_WINLOSE),
							  MeshBuilder::GenerateXYPlane("Image//go.tga", 12.0f, 6.75f, 1, type::SHADER_WINLOSE));
	winLoseGraphic = new WinLoseGraphic(MeshBuilder::GenerateXYPlane("Image//finish.tga", 12.0f, 6.75f, 1, type::SHADER_WINLOSE),
										MeshBuilder::GenerateXYPlane("Image//winner.tga", 6.0f, 3.375f, 1, type::SHADER_WINLOSE),
										MeshBuilder::GenerateXYPlane("Image//loser.tga", 6.0f, 3.375f, 1, type::SHADER_WINLOSE));

	timer = new Timer();

	glViewport(0, 0, width * 0.5, height);
	StartView(player[0]);

	glViewport(0, height*0.5, width *0.5, height);
	StartView(player[1]);

	glViewport(0, 0, width, height);

	// Enable culling and depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	isCullFace = true;

	countdown->start();
}

void SceneGame::Update(double dt, GLFWwindow * programID)
{
	// Update deltaTime
	deltaTime = dt;
	// Process keyboard input
	processInput(programID);
	
	Map->update(programID, dt);

	Physics::physicsEngine.update();
	ModGate::detector.update();
	handleLap->update();

	winLoseGraphic->registerWin(handleLap->getWinner());
}

void SceneGame::Render()
{
	// Clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, width * 0.5, height);
	UpdateView(player[0]);
	renderView(0);

	placing++;
	glViewport(width * 0.5, 0, width * 0.5, height);
	UpdateView(player[1]);
	renderView(1);

	glViewport(0, 0, width, height);
	Mtx44 projection;
	projection.SetToOrtho(0, 20, 0, 20, 0, 0.01);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixP);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), projection.a);

	// Render text
	text->PrintTextForward("FPS:" + calculateFPS(), uMatrixMVS, 0.0f,19.f, 1.0f);
	text->PrintTextBackward("Elapsed:" + timer->getTimeText() + "s", uMatrixMVS, 19.0f, 19.0f, 1.0f);
	countdown->render(uMatrixMVS);
	winLoseGraphic->render(uMatrixMVS);

	// Reset projection
	projection.SetToPerspective(55.0, static_cast<double>(width) / 2.0 / static_cast<double>(height), 0.1, 100.0);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixP);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), projection.a);
	
}

void SceneGame::Exit()
{
	for (int i = 0; i < NO_OF_POINTLIGHTS; ++i)
	{
		delete lamp[i];
	}
	delete axes;
	delete floor;
	delete hotbar;
	delete skybox;
	for (int i = 0; i < 2; ++i)
	{
		delete player[i];
	}
	for (int i = 0; i < 3; ++i)
	{
		delete GUI[i];
	}
	delete handleLap;
	delete playerDummy;
	delete text;
	delete Map;
	delete winLoseGraphic;
	delete countdown;
	delete timer;

	// Free memory allocated for UBOs
	glDeleteBuffers(1, &uMatrixMVS);
	glDeleteBuffers(1, &uMatrixP);
	glDeleteBuffers(1, &uColorData);
	glDeleteBuffers(1, &uSpotLight);
}

void SceneGame::renderView(unsigned int view)
{
	Mtx44 offset;
	offset.SetToIdentity();
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), offset.a);



	skybox->draw(); // Draw skybox first

	axes->draw(); // Draw the axes
	floor->render(player[view]->getCam().pos, uMatrixMVS); // Draw the floor
	for (int i = 0; i < 2; ++i)
	{
		player[i]->render(uMatrixMVS);
	}
	//player->render(); // Draw player (if applicable)
	objectList.renderObjects(uMatrixMVS);

	// Draw the lamps
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), offset.a);
	for (int i = 0; i < NO_OF_POINTLIGHTS; ++i)
	{
		offset.SetToTranslation(lamp[i]->light.position.x, lamp[i]->light.position.y, lamp[i]->light.position.z);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), offset.a);
		lamp[i]->draw();
	}
	// Prepare projection matrix for HUD rendering
	Mtx44 projection;
	projection.SetToOrtho(0, 20, 0, 20, 0, 0.01);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixP);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), projection.a);
	text->PrintTextBackward(player[view]->getCar()->getSpeedText(), uMatrixMVS, 19.0f, 2.5f, 1.0f);
	text->PrintTextBackward("Gear:" + player[view]->getCar()->getGear(), uMatrixMVS, 19.0f, 3.5f, 1.0f);
	MS model;
	model.LoadIdentity();
	model.PushMatrix();
	model.Translate(2.0f, 1.5f, 0.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	GUI[0]->Render();
	
	model.Translate(15.0f, 0.0f, 0.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	GUI[handleLap->getPlacing(player[view]->getCar())]->Render();
	placing = 1;
	model.PopMatrix();
	

	//item player holding now
	model.PushMatrix();
	model.LoadIdentity();
	model.Translate(2.0f, 1.5f, 0.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
		switch (player[view]->getCar()->getStatus())
		{

		    case Kart::e_basic:
				if (player[view]->getCar()->get_used())
				{
					GUI[3]->Render();
					break;
				}
				else
				{
					GUI[3]->Render();
					break;
				}	
	     	case Kart::e_pikachu:
				if (player[view]->getCar()->get_used())
				{
					GUI[3]->Render();
					break;
				}
				else
				{
					GUI[4]->Render();
					break;
				}	
			case Kart::e_eevee:
				if (player[view]->getCar()->get_used())
				{
					GUI[3]->Render();
					break;
				}
				else
				{
					GUI[5]->Render();
					break;
				}
			case Kart::e_mew:
				if (player[view]->getCar()->get_used())
				{
					GUI[3]->Render();
					break;
				}
				else
				{
					GUI[6]->Render();
					break;
				}
			case Kart::e_squirtle:
				if (player[view]->getCar()->get_used())
				{
					GUI[3]->Render();
					break;
				}
				else
				{
					GUI[7]->Render();
					break;
				}
			default:
				break;
	    }
		model.PopMatrix();
	////////

		//////current debuff
		model.PushMatrix();
		model.LoadIdentity();
		model.Translate(10.0f, 15.0f, 0.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);		
		switch (player[view]->getCar()->getBuff())
		{			
		case Kart::b_stun:
			GUI[4]->Render();
			break;
		case Kart::b_speed:
			GUI[5]->Render();
			break;
		case Kart::b_confuse:
			GUI[6]->Render();
			break;
		case Kart::b_slow:
			GUI[7]->Render();
			break;
		case Kart::b_nothing:
			break;
		default:
			break;
		}
		model.PopMatrix();



	// Reset projection
	projection.SetToPerspective(55.0, static_cast<double>(width) / 2.0 / static_cast<double>(height), 0.1, 100.0);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixP);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), projection.a);
	
}

void SceneGame::StartView(c_m_Player * player)
{
	// Initialize model, view, projection matrices within shaders
	Mtx44 MVP[3];
	MVP[0].SetToIdentity();
	MVP[1].SetToLookAt(player->getCam().pos.x, player->getCam().pos.y, player->getCam().pos.z,
		player->getCam().pos.x + player->getCam().front.x, player->getCam().pos.y + player->getCam().front.y, player->getCam().pos.z + player->getCam().front.z,
		player->getCam().up.x, player->getCam().up.y, player->getCam().up.z);
	MVP[2].SetToPerspective(55.0, static_cast<double>(width) / 2.0 / static_cast<double>(height), 0.1, 100.0);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 2 * sizeof(Mtx44), MVP[0].a);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixP);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), MVP[2].a);

	// Initialize each lamp
	for (int i = 0; i < NO_OF_POINTLIGHTS; ++i)
	{
		lamp[i] = new Lamp(uColorData);
		lamp[i]->light.position = Vector3(0.0f, 10.0f, -10.0f);
		lamp[i]->light.ambient = Vector3(0.1f, 0.1f, 0.1f);
		lamp[i]->light.diffuse = Vector3(0.7f, 0.7f, 0.7f);
		lamp[i]->light.specular = Vector3(0.3f, 0.3f, 0.3f);
		lamp[i]->light.constant = 0.6f;
		lamp[i]->light.linear = 0.009f;
		lamp[i]->light.quadratic = 0.0f;
	}

	// Initialize each point light within shaders
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	for (int i = 0; i < NO_OF_POINTLIGHTS; ++i)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44) + 16 + 80 * i, 12, &lamp[i]->light.position.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44) + 16 + 80 * i + 12, 4, &lamp[i]->light.constant);
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44) + 16 + 80 * i + 16, 4, &lamp[i]->light.linear);
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44) + 16 + 80 * i + 20, 4, &lamp[i]->light.quadratic);
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44) + 16 + 80 * i + 32, 12, &lamp[i]->light.ambient.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44) + 16 + 80 * i + 48, 12, &lamp[i]->light.diffuse.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44) + 16 + 80 * i + 64, 12, &lamp[i]->light.specular.x);
	}
}

void SceneGame::UpdateView(c_m_Player * player)
{
	// Update shaders with new view matrix
	Mtx44 view;
	view.SetToLookAt(player->getCam().pos.x, player->getCam().pos.y, player->getCam().pos.z,
		player->getCam().pos.x + player->getCam().front.x, player->getCam().pos.y + player->getCam().front.y, player->getCam().pos.z + player->getCam().front.z,
		player->getCam().up.x, player->getCam().up.y, player->getCam().up.z);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Mtx44), sizeof(Mtx44), view.a);

	// Update point light position within shaders
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44) + 16, 12, &lamp[0]->light.position.x);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SceneGame::processInput(GLFWwindow * window)
{
	// Check window close
	if (isPressed(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Update the player and kart(if any)
	if (!countdown->isCounting())
	{
		player[0]->update(window, deltaTime, uSpotLight);
		player[1]->update(window, deltaTime, uSpotLight);
	}

	// Misc.
	if (isPressed(window, GLFW_KEY_1)) // Enable cull face
	{
		glEnable(GL_CULL_FACE);
		isCullFace = true;
	}
	else if (isPressed(window, GLFW_KEY_2)) // Disable cull face
	{
		glDisable(GL_CULL_FACE);
		isCullFace = false;
	}
	if (isPressed(window, GLFW_KEY_3)) // Disable wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (isPressed(window, GLFW_KEY_4)) // Enable wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void SceneGame::framebuffer_resize_callback(GLFWwindow * window, int width, int height)
{
	// Resize viewport
	glViewport(0, 0, width, height);

	SceneGame::width = width;
	SceneGame::height = height;
}

std::string SceneGame::calculateFPS() const
{
	std::stringstream convert;
	convert.precision(2);

	convert << std::fixed << 1.0 / deltaTime;
	return convert.str();
}

//void SceneGame::drawCoordinates() const
//{
//	std::stringstream convertX, convertY, convertZ;
//
//	convertX.precision(1);
//	convertX << std::fixed << player->getCam().pos.x;
//	text->PrintTextForward("x:" + convertX.str(), uMatrixMVS, 0.0f, 19.0f, 1.0f);
//
//	convertY.precision(1);
//	convertY << std::fixed << player->getCam().pos.y;
//	text->PrintTextForward("y:" + convertY.str(), uMatrixMVS, 0.0f, 18.0f, 1.0f);
//
//	convertZ.precision(1);
//	convertZ << std::fixed << player->getCam().pos.z;
//	text->PrintTextForward("z:" + convertZ.str(), uMatrixMVS, 0.0f, 17.0f, 1.0f);
//
//	text->PrintTextForward(std::to_string(Physics::physicsEngine.testCollision()), uMatrixMVS, 0, 15, 1);
//}


