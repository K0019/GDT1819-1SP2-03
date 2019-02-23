#include "ScenePauseMenu.h"

int ScenePauseMenu::width = 800, ScenePauseMenu::height = 600;

ScenePauseMenu::ScenePauseMenu(short noOfButtons)
	: MenuSelection(noOfButtons)
	, stay(true)
	, exitToMainMenu(false)
{

}

ScenePauseMenu::~ScenePauseMenu()
{

}

void ScenePauseMenu::resize(int width, int height)
{
	ScenePauseMenu::width = width;
	ScenePauseMenu::height = height;
}

bool ScenePauseMenu::shouldExit() const
{
	return stay;
}

bool ScenePauseMenu::shouldExitToMainMenu() const
{
	return exitToMainMenu;
}