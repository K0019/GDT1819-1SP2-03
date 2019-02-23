#ifndef SCENEPAUSEMENUHEADTOHEAD_H
#define SCENEPAUSEMENUHEADTOHEAD_H
#include "ScenePauseMenu.h"
#include "SceneGame.h"
#include "MenuSelection.h"

class ScenePauseMenuHeadToHead : public ScenePauseMenu
{
public:
	ScenePauseMenuHeadToHead(Scene* scene);
	~ScenePauseMenuHeadToHead();

	virtual void Init() override;
	virtual void Update(double dt, GLFWwindow* window) override;
	virtual void Render() override;
	virtual void Exit() override;

protected:
	virtual void activated(short selection) override;

private:
	SceneGame* scene;

	unsigned int uMatrixMVS, uMatrixP;
};

#endif // !SCENEPAUSEMENUHEADTOHEAD_H
