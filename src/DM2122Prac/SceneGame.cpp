#include "SceneGame.h"



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

	// Use most general shader for configuration
	shader::container.useShader(type::SHADER_2);
	m_programID = shader::container.getID(type::SHADER_2);


}

void SceneGame::Update(double dt, GLFWwindow * programID)
{

}

void SceneGame::Render()
{
}

void SceneGame::Exit()
{
}

void SceneGame::processInput(GLFWwindow * window)
{
	// Check window close
	if (isPressed(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Update the player and kart(if any)
	player[0]->update(window, deltaTime, uSpotLight);
	player[1]->update(window, deltaTime, uSpotLight);

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
