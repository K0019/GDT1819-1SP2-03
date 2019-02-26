#include "SceneMainMenu.h"

int SceneMainMenu::width = PROGRAM_WIDTH, SceneMainMenu::height = PROGRAM_HEIGHT;

SceneMainMenu::SceneMainMenu()
	: MenuSelection(4)
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
	showHelp = false;

	icons[0] = MeshBuilder::GenerateXYPlane("Image//game_icon.tga", 15.0f, 8.0f, 1, type::SHADER_TEXT);
	icons[1] = MeshBuilder::GenerateXYPlane("Image//help_menu.tga", 20.0f, 15.0f, 1, type::SHADER_TEXT);
}

void SceneMainMenu::Update(double dt, GLFWwindow* window)
{
	if (!isPressed(window, GLFW_KEY_ENTER))
	{
		ignoreEnter = false;
	}
	if (!showHelp)
	{
		MenuSelection::update(dt, window);
	}
	else
	{
		if (!ignoreEnter)
		{
			if (isPressed(window, GLFW_KEY_ENTER))
			{
				ignoreEnter = true;
				showHelp = false;
			}
		}
	}
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

	if (showHelp)
	{
		model.PushMatrix();
		model.Translate(0.0f, 10.0f, 0.0f);
		updateUBO(uMatrixMVS, 0, sizeof(Mtx44), model.Top().a);
		icons[1]->Render();
		model.PopMatrix();

		model.PushMatrix();
		model.Translate(0.0, 2.0f, 0.0f);
		updateUBO(uMatrixMVS, 0, sizeof(Mtx44), model.Top().a);
		render(2);
		model.PopMatrix();
		text->PrintTextForward("Back", uMatrixMVS, -1.35f, 1.8f, 0.9f);
	}
	else
	{
		model.PushMatrix(); // 1
		model.Translate(0.0f, 15.0f, 0.0f);
		updateUBO(uMatrixMVS, 0, sizeof(Mtx44), model.Top().a);
		icons[0]->Render();
		model.PopMatrix(); // 1

		model.Translate(0.0f, 9.5f, 0.0f);

		for (int i = 0; i < 4; ++i)
		{
			model.PushMatrix(); // 1
			model.Translate(0.0f, static_cast<float>(-i) * 2.5f, 0.0f);
			updateUBO(uMatrixMVS, 0, sizeof(Mtx44), model.Top().a);
			render(i);
			model.PopMatrix(); // 1

			switch (i)
			{
			case 0:
				text->PrintTextForward("Multiplayer", uMatrixMVS, -3.65f, 10.05f - static_cast<float>(i) * 2.775f, 0.9f);
				break;
			case 1:
				text->PrintTextForward("Level Editor", uMatrixMVS, -3.95f, 10.05f - static_cast<float>(i) * 2.775f, 0.9f);
				break;
			case 2:
				text->PrintTextForward("Help", uMatrixMVS, -1.35f, 10.05f - static_cast<float>(i) * 2.775f, 0.9f);
				break;
			case 3:
				text->PrintTextForward("Quit", uMatrixMVS, -1.35f, 10.05f - static_cast<float>(i) * 2.775f, 0.9f);
				break;
			}
		}
	}

	model.PopMatrix(); // 0
}

void SceneMainMenu::Exit()
{
	for (int i = 0; i < 2; ++i)
	{
		delete icons[i];
	}

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
			showHelp = true;
			break;
		case 3:
			gamemode = EXIT;
			break;
		}
	}
	ignoreEnter = true;
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