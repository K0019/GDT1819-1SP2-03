#ifndef SCENEMAINMENU_H
#define SCENEMAINMENU_H
#include "Source/Scene.h"
#include "MenuSelection.h"
#include "MatrixStack.h"

class SceneMainMenu : public Scene, public MenuSelection
{
public:
	enum Gamemode
	{
		EXIT,
		MAIN_MENU,
		LEVEL_EDITOR,
		HEAD_TO_HEAD
	};

	SceneMainMenu();
	~SceneMainMenu();

	virtual void Init() override;
	virtual void Update(double dt, GLFWwindow* window) override;
	virtual void Render() override;
	virtual void Exit() override;


	Gamemode getGamemode() const; // Get currently selected gamemode
	void setBackToMainMenu();

	static void resize(int width, int height);
protected:
	virtual void activated(short selection) override;

private:
	Mesh* icons[2];

	Gamemode gamemode;
	bool ignoreEnter;

	unsigned int uMatrixMVS, uMatrixP;

	static int width, height;
};

#endif // !SCENEMAINMENU_H
