#include "WinLoseGraphic.h"

WinLoseGraphic::WinLoseGraphic(Mesh* win, Mesh* lose)
	: win(win)
	, lose(lose)
	, finished(false)
	, timer(nullptr)
{

}

WinLoseGraphic::~WinLoseGraphic()
{
	delete timer;
}

void WinLoseGraphic::registerWin(int winnerID)
{
	if (!finished && winnerID != -1)
	{
		this->winnerID = winnerID;
		finished = true;
		timer = new StopWatch();
		timer->startTimer();
	}
}

void WinLoseGraphic::render(unsigned int uMatrixMVS) const
{
	if (finished)
	{
		MS model;
		model.PushMatrix();
		model.LoadIdentity();

		model.PushMatrix();
		model.Translate(5.0f, 10.0f, 0.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
		if (winnerID == 0)
		{
			win->Render();
		}
		else
		{
			lose->Render();
		}
		model.PopMatrix();

		model.PushMatrix();
		model.Translate(15.0f, 10.0f, 0.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
		if (winnerID == 1)
		{
			win->Render();
		}
		else
		{
			lose->Render();
		}
		model.PopMatrix();

		model.PopMatrix();
	}
}