#include "ScenePauseMenuLevelEditor.h"

ScenePauseMenuLevelEditor::ScenePauseMenuLevelEditor(Scene* scene)
	: ScenePauseMenu(5)
{
	Scene2* castScene = dynamic_cast<Scene2*>(scene);
	if (!castScene)
		throw std::exception("Failed to cast Scene* to Scene2* in ScenePauseMenuLevelEditor");

	this->scene = castScene;
}

ScenePauseMenuLevelEditor::~ScenePauseMenuLevelEditor()
{
}

void ScenePauseMenuLevelEditor::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

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
}

void ScenePauseMenuLevelEditor::Update(double dt, GLFWwindow* window)
{
	MenuSelection::update(dt, window);
}

void ScenePauseMenuLevelEditor::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mtx44 projection;
	projection.SetToOrtho(-10.0 * static_cast<double>(width) / static_cast<double>(height), 10.0 * static_cast<double>(width) / static_cast<double>(height), 0.0, 20.0, -0.1, 0.1);
	updateUBO(uMatrixP, 0, sizeof(Mtx44), projection.a);

	MS model;
	model.PushMatrix(); // 0
	model.LoadIdentity();
	model.Translate(0.0f, 14.5f, 0.0f);

	for (int i = 0; i < 5; ++i)
	{
		model.PushMatrix(); // 1
		model.Translate(0.0f, static_cast<float>(-i) * 2.5f, 0.0f);
		updateUBO(uMatrixMVS, 0, sizeof(Mtx44), model.Top().a);
		render(i);
		model.PopMatrix(); // 1

		switch (i)
		{
		case 0:
			text->PrintTextForward("Resume", uMatrixMVS, -2.0f, 16.3f - static_cast<float>(i) * 2.775f, 0.9f);
			break;
		case 1:
			text->PrintTextForward("Save", uMatrixMVS, -1.3f, 16.3f - static_cast<float>(i) * 2.775f, 0.9f);
			break;
		case 2:
			text->PrintTextForward("Load", uMatrixMVS, -1.3f, 16.3f - static_cast<float>(i) * 2.775f, 0.9f);
			break;
		case 3:
			text->PrintTextForward("Clear", uMatrixMVS, -1.5f, 16.3f - static_cast<float>(i) * 2.775f, 0.9f);
			break;
		case 4:
			text->PrintTextForward("Main Menu", uMatrixMVS, -3.0f, 16.3f - static_cast<float>(i) * 2.775f, 0.9f);
			break;
		}
	}

	model.PopMatrix(); // 0
}

void ScenePauseMenuLevelEditor::Exit()
{
	glDeleteBuffers(1, &uMatrixMVS);
	glDeleteBuffers(1, &uMatrixP);
}

void ScenePauseMenuLevelEditor::activated(short selection)
{
	switch (selection)
	{
	case 0: // Resume
		stay = false;
		break;
	case 1: // Save
		scene->saveMap();
		
		break;
	case 2: // Load
		scene->loadMap();
	
		break;
	case 3: // Clear
		scene->clearMap();
		
		break;
	case 4: // Main Menu
		stay = false;
		exitToMainMenu = true;
		break;
	}
}