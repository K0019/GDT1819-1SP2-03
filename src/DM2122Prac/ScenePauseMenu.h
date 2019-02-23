#ifndef SCENEPAUSEMENU_H
#define SCENEPAUSEMENU_H
#include "Source/Scene.h"
#include "MenuSelection.h"

class ScenePauseMenu : public Scene, public MenuSelection
{
public:
	ScenePauseMenu(short noOfButtons);
	~ScenePauseMenu();

	static void resize(int width, int height);

	bool shouldExit() const; // Get whether to stay in pause menu
	bool shouldExitToMainMenu() const; // Get whether to stay in main menu
protected:
	static int width, height;

	bool stay; // Determine whether to stay in pause menu
	bool exitToMainMenu; // Determine whether to exit to main menu

	unsigned int uMatrixMVS, uMatrixP;
private:
};

#endif // !SCENEPAUSEMENU_H
