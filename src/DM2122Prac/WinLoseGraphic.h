#ifndef WINLOSEGRAPHIC_H
#define WINLOSEGRAPHIC_H
#include "Source/Mesh.h"
#include "timer.h"
#include "Source/ShaderPrograms.h"
#include "MatrixStack.h"

class WinLoseGraphic
{
public:
	WinLoseGraphic(Mesh* finish, Mesh* win, Mesh* lose);
	~WinLoseGraphic();

	void registerWin(int winnerID);

	void render(unsigned int uMatrixMVS);
protected:

private:
	Mesh* finish, *win, *lose;

	bool finished, showFinish;
	int winnerID;

	StopWatch* timer;
	double totalTime;
};

#endif // !WINLOSEGRAPHIC_H
