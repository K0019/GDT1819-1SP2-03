#include "WinLoseGraphic.h"

WinLoseGraphic::WinLoseGraphic(Mesh* finish, Mesh* win, Mesh* lose)
	: finish(finish)
	, win(win)
	, lose(lose)
	, finished(false)
	, showFinish(false)
	, timer(nullptr)
	, totalTime(0.0)
{

}

WinLoseGraphic::~WinLoseGraphic()
{
	delete finish;
	delete win;
	delete lose;

	delete timer;
}

void WinLoseGraphic::registerWin(int winnerID)
{
	if (!showFinish && !finished && winnerID != -1)
	{
		this->winnerID = winnerID;
		showFinish = true;
		timer = new StopWatch();
		timer->startTimer();
	}
}

void WinLoseGraphic::render(unsigned int uMatrixMVS, float width, float height)
{
	if (showFinish)
	{
		// Finish main
		totalTime += timer->getElapsedTime();
		shader::container.useShader(type::SHADER_WINLOSE);
		float opacity, size;

		MS model;
		model.PushMatrix();
		model.LoadIdentity();

		model.PushMatrix();
		model.Translate(0.0f, 10.0f, 0.0f);

		// Fading finishes
		for (int i = 0; i < 3; ++i)
		{
			float time = static_cast<float>(totalTime - 0.5) - static_cast<float>(i) * 0.5f;
			opacity = 1.0f - 1.333f * time;
			if (opacity > 1.0f || opacity < 0.0f)
				continue;
			size = 1.0f + time * 0.5f;

			model.PushMatrix();
			model.Scale(size, size, size);
			glUniform1f(glGetUniformLocation(shader::container.getID(type::SHADER_WINLOSE), "alpha"), opacity);
			glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);

			finish->Render();
			model.PopMatrix();
		}

		opacity = (totalTime < 1.5 ? (totalTime < 0.5 ? static_cast<float>(totalTime * 2.0) : 1.0f) : 0.0f);
		size = (totalTime < 0.5 ? 1.5f - static_cast<float>(totalTime) : 1.0f);
		glUniform1f(glGetUniformLocation(shader::container.getID(type::SHADER_WINLOSE), "alpha"), opacity);

		model.PushMatrix();
		model.Scale(size, size, size);
		glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
		finish->Render();
		model.PopMatrix();

		model.PopMatrix();
		model.PopMatrix();

		if (totalTime >= 3.5)
		{
			totalTime -= 3.5;
			finished = true;
			showFinish = false;
		}
	}
	else if (finished)
	{
		totalTime += timer->getElapsedTime();
		shader::container.useShader(type::SHADER_WINLOSE);
		glUniform1f(glGetUniformLocation(shader::container.getID(type::SHADER_WINLOSE), "alpha"), static_cast<float>(totalTime * 1.5));
		float size = static_cast<float>((totalTime < 1.0 ? totalTime : 1.0));

		MS model;
		model.PushMatrix();
		model.LoadIdentity();

		model.PushMatrix();
		model.Translate(-5.0f * width / height, 10.0f, 0.0f);
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
		model.Translate(5.0f * width / height, 10.0f, 0.0f);
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