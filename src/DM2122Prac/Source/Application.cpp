//! Comments without ! are comments included in the framework
//! Comments with ! are comments added by me

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>
#include <memory>

#include "Application.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
//! Moved to Scene1 (processInput())

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

// Function called when window resizes to adapt render viewport
void Application::framebuffer_resize_callback(GLFWwindow* window, int width, int height)
{
	// Resize viewport
	glViewport(0, 0, width, height);

	Scene2::resize(width, height);
	SceneGame::resize(width, height);
	SceneMainMenu::resize(width, height);
	ScenePauseMenu::resize(width, height);
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(PROGRAM_WIDTH, PROGRAM_HEIGHT, "Assignment 2", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}
	err = glGetError();

	//! Initialize shaders
	shader::container.init();
}

void Application::Run()
{
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //! Using mouse cursor movement
	//Main Loop
	SceneMainMenu* mainMenu = new SceneMainMenu();
	mainMenu->Init();
	/*Scene *scene = new SceneGame();
	scene->Init();*/
	//glfwSetKeyCallback(m_window, Scene1::key_callback);
	//glfwSetCursorPosCallback(m_window, Player::mouse_callback); //! Set function to call when mouse moves
	//glfwSetScrollCallback(m_window, Hotbar::scroll_callback); //! Set function to call when mouse wheel is scrolled
	glfwSetFramebufferSizeCallback(m_window, Application::framebuffer_resize_callback); //! Set function to call when window is resized

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window))
	{
		while (mainMenu->getGamemode() == SceneMainMenu::Gamemode::MAIN_MENU) // Main Menu loop
		{
			sceneIteration(mainMenu);
		}
		if (mainMenu->getGamemode() == SceneMainMenu::Gamemode::EXIT)
			break;

		// Switch scene
		Scene* scene;
		switch (mainMenu->getGamemode())
		{
		case SceneMainMenu::Gamemode::LEVEL_EDITOR:
			scene = new Scene2();
			glfwSetCursorPosCallback(m_window, Player::mouse_callback);
			glfwSetScrollCallback(m_window, Hotbar::scroll_callback);
			break;
		case SceneMainMenu::Gamemode::HEAD_TO_HEAD:
			scene = new SceneGame();
			break;
		}
		scene->Init();

		bool exitToMainMenu = false;
		while (!exitToMainMenu) // Scene loop
		{
			sceneIteration(scene);

			if (isPressed(m_window, GLFW_KEY_ESCAPE))
			{
				ScenePauseMenu* pauseScene;
				// Pause
				switch (mainMenu->getGamemode())
				{
				case SceneMainMenu::Gamemode::LEVEL_EDITOR:
					pauseScene = new ScenePauseMenuLevelEditor(scene);
					break;
				case SceneMainMenu::Gamemode::HEAD_TO_HEAD:
					pauseScene = new ScenePauseMenuHeadToHead(scene);
					break;
				}
				pauseScene->Init();

				while (pauseScene->shouldExit() && !pauseScene->shouldExitToMainMenu())
				{
					sceneIteration(pauseScene);
				}

				exitToMainMenu = pauseScene->shouldExitToMainMenu();

				pauseScene->Exit();
				delete pauseScene;
			}
		}
		scene->Exit();
		delete scene;
		Physics::physicsEngine.empty();
		mainMenu->setBackToMainMenu();
	} //Check if the ESC key had been pressed or if the window had been closed
	mainMenu->Exit();
	delete mainMenu;
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}

void Application::sceneIteration(Scene* scene)
{
	scene->Update(m_timer.getElapsedTime(), m_window);
	scene->Render();
	//Swap buffers
	glfwSwapBuffers(m_window);
	//Get and organize events, like keyboard and mouse input, window resizing, etc...
	glfwPollEvents();
	m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   
}