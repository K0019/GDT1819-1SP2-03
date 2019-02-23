#ifndef SCENEPAUSEMENULEVELEDITOR_H
#define SCENEPAUSEMENULEVELEDITOR_H
#include "ScenePauseMenu.h"
#include "Source/Scene2.h"
#include "Sound.h"
#include "UsefulFunctions.h"

class ScenePauseMenuLevelEditor : public ScenePauseMenu
{
public:
	ScenePauseMenuLevelEditor(Scene* scene);
	~ScenePauseMenuLevelEditor();

	virtual void Init() override;
	virtual void Update(double dt, GLFWwindow* window) override;
	virtual void Render() override;
	virtual void Exit() override;

protected:
	virtual void activated(short selection) override;

private:
	Scene2* scene;

	unsigned int uMatrixMVS, uMatrixP;
};

#endif // !SCENEPAUSEMENULEVELEDITOR_H
