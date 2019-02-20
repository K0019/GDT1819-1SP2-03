#ifndef APPLICATION_H
#define APPLICATION_H
#include "timer.h"
#include "Scene2.h"
#include "SceneGame.h"
/* File: Application.h
   Use: Initializes and contains the OpenGL program 
		Framework file, lightly modified */

class Application
{
public:
	Application(); // Constructor
	~Application(); // Destructor
	void Init(); // Initialize function
	void Run(); // Function that loops until program closes
	void Exit(); // Cleanup function
	bool IsKeyPressed(unsigned short key); // UNUSED - Checks whether a specified virtual key is pressed

private:
	StopWatch m_timer; // Program runtime timer
};

#endif