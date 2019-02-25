#include "SceneMainMenu.h"

int SceneMainMenu::width = PROGRAM_WIDTH, SceneMainMenu::height = PROGRAM_HEIGHT;

SceneMainMenu::SceneMainMenu()
	: MenuSelection(3)
{
	music::player.init();
}

SceneMainMenu::~SceneMainMenu()
{
	
}

void SceneMainMenu::resize(int width, int height)
{
	SceneMainMenu::width = width;
	SceneMainMenu::height = height;
}

void SceneMainMenu::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
	music::player.setsoundvol(0.15);
	music::player.playsound(music::player.getMainSound(), true);
	glGenBuffers(1, &uMatrixMVS);
	glGenBuffers(1, &uMatrixP);

	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(Mtx44) + 16 + 80 * NO_OF_POINTLIGHTS, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uMatrixMVS);

	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixP);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Mtx44), NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, uMatrixP);

	Mtx44 MVP[3];
	MVP[0].SetToIdentity();
	MVP[1].SetToIdentity();
	MVP[2].SetToOrtho(-10.0 * static_cast<double>(width) / static_cast<double>(height), 10.0 * static_cast<double>(width) / static_cast<double>(height), 0.0, 20.0, -0.1, 0.1);
	updateUBO(uMatrixMVS, 0, 2 * sizeof(Mtx44), MVP[0].a);
	updateUBO(uMatrixP, 0, sizeof(Mtx44), MVP[2].a);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);



	gamemode = MAIN_MENU;
	ignoreEnter = false;
}

void SceneMainMenu::Update(double dt, GLFWwindow* window)
{
	if (!isPressed(window, GLFW_KEY_ENTER))
	{
		ignoreEnter = false;
	}
	MenuSelection::update(dt, window);
}

void SceneMainMenu::Render()
{
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uMatrixMVS);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, uMatrixP);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mtx44 projection;
	projection.SetToOrtho(-10.0 * static_cast<double>(width) / static_cast<double>(height), 10.0 * static_cast<double>(width) / static_cast<double>(height), 0.0, 20.0, -0.1, 0.1);
	updateUBO(uMatrixP, 0, sizeof(Mtx44), projection.a);

	MS model;
	model.PushMatrix(); // 0
	model.LoadIdentity();
	model.Translate(0.0f, 7.0f, 0.0f);

	for (int i = 0; i < 3; ++i)
	{
		model.PushMatrix(); // 1
		model.Translate(0.0f, static_cast<float>(-i) * 2.5f, 0.0f);
		updateUBO(uMatrixMVS, 0, sizeof(Mtx44), model.Top().a);
		render(i);
		model.PopMatrix(); // 1

		switch (i)
		{
		case 0:
			text->PrintTextForward("Multiplayer", uMatrixMVS, -3.65f, 7.3f - static_cast<float>(i) * 2.775f, 0.9f);
			break;
		case 1:
			text->PrintTextForward("Level Editor", uMatrixMVS, -3.95f, 7.3f - static_cast<float>(i) * 2.775f, 0.9f);
			break;
		case 2:
			text->PrintTextForward("Quit", uMatrixMVS, -1.35f, 7.3f - static_cast<float>(i) * 2.775f, 0.9f);
			break;
		}
	}

	model.PopMatrix(); // 0
}

void SceneMainMenu::Exit()
{
	glDeleteBuffers(1, &uMatrixMVS);
	glDeleteBuffers(1, &uMatrixP);
}

void SceneMainMenu::activated(short selection)
{
	if (!ignoreEnter)
	{
		music::player.getEngine()->removeSoundSource(music::player.getMainSound());
		switch (selection)
		{
		case 0:
			gamemode = HEAD_TO_HEAD;
			break;
		case 1:
			gamemode = LEVEL_EDITOR;
			break;
		case 2:
			gamemode = EXIT;
			break;
		}
	}
}

SceneMainMenu::Gamemode SceneMainMenu::getGamemode() const
{
	return gamemode;
}

void SceneMainMenu::setBackToMainMenu()
{
	
	music::player.getEngine()->removeAllSoundSources();
	music::player.init();
	music::player.playsound(music::player.getMainSound(), true);
	gamemode = MAIN_MENU;
	ignoreEnter = true;
}