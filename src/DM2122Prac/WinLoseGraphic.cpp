#include "WinLoseGraphic.h"

WinLoseGraphic::WinLoseGraphic(Mesh* win, Mesh* lose)
	: win(win)
	, lose(lose)
	, finished(false)
	, timer(nullptr)
	, totalTime(0.0)
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

void WinLoseGraphic::render(unsigned int uMatrixMVS)
{
	if (finished)
	{
		totalTime += timer->getElapsedTime();
		shader::container.useShader(type::SHADER_WINLOSE);
		glUniform1f(glGetUniformLocation(shader::container.getID(type::SHADER_WINLOSE), "alpha"), static_cast<float>(totalTime * 1.5));
		float size = static_cast<float>((totalTime < 1.0 ? totalTime : 1.0));

		MS model;
		model.PushMatrix();
		model.LoadIdentity();

		model.PushMatrix();
		model.Translate(5.0f, 10.0f, 0.0f);
		model.Scale(2.0f - size, 2.0f - size, 2.0f - size);
		glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
		if (winnerID == 0)
		{
			if (totalTime > 1.0)
			{
				float opacity = static_cast<float>((1.0 - 2.0 * fmod(totalTime, 1.0) > 0.0 ? 1.0 - 2.0 * fmod(totalTime, 1.0) : 0.0));
				float size = 1.5f - 0.5 * opacity;
				model.PushMatrix();
				model.Scale(size, size, size);
				glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
				glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
				glUniform1f(glGetUniformLocation(shader::container.getID(type::SHADER_WINLOSE), "alpha"), opacity);
				win->Render();
				model.PopMatrix();
				glUniform1f(glGetUniformLocation(shader::container.getID(type::SHADER_WINLOSE), "alpha"), static_cast<float>(totalTime * 1.5));
				glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
				glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
			}
			win->Render();
		}
		else
		{
			lose->Render();
		}
		model.PopMatrix();

		model.PushMatrix();
		model.Translate(15.0f, 10.0f, 0.0f);
		model.Scale(2.0f - size, 2.0f - size, 2.0f - size);
		glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
		if (winnerID == 1)
		{
			if (totalTime > 1.0)
			{
				float opacity = static_cast<float>((1.0 - 2.0 * fmod(totalTime, 1.0) > 0.0 ? 1.0 - 2.0 * fmod(totalTime, 1.0) : 0.0));
				float size = 1.5f - 0.5 * opacity;
				model.PushMatrix();
				model.Scale(size, size, size);
				glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
				glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
				glUniform1f(glGetUniformLocation(shader::container.getID(type::SHADER_WINLOSE), "alpha"), opacity);
				win->Render();
				model.PopMatrix();
				glUniform1f(glGetUniformLocation(shader::container.getID(type::SHADER_WINLOSE), "alpha"), static_cast<float>(totalTime * 1.5));
				glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
				glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
			}
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