#include "Countdown.h"

Countdown::Countdown(Mesh* three, Mesh* two, Mesh* one, Mesh* go)
	: totalTime(0.0)
{
	meshes = new Mesh*[4];
	meshes[0] = three;
	meshes[1] = two;
	meshes[2] = one;
	meshes[3] = go;
	timer = new StopWatch();
}

Countdown::~Countdown()
{
	delete[] meshes;
	delete timer;
}

void Countdown::start()
{
	timer->startTimer();
}

void Countdown::render(unsigned int uMatrixMVS)
{
	totalTime += timer->getElapsedTime();

	if (totalTime < 4.0)
	{
		shader::container.useShader(type::SHADER_WINLOSE);
		float opacity = 1.0f - static_cast<float>(fmod(totalTime, 1.0));
		float size = 1.0f + static_cast<float>(fmod(totalTime, 1.0));

		MS model;
		model.PushMatrix();
		model.LoadIdentity();

		model.PushMatrix();
		model.Translate(10.0f, 10.0f, 0.0f);
		model.Scale(size, size, size);

		glUniform1f(glGetUniformLocation(shader::container.getID(type::SHADER_WINLOSE), "alpha"), opacity);
		glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);

		meshes[static_cast<int>(totalTime)]->Render();

		model.PopMatrix();
		model.PopMatrix();
	}
}

bool Countdown::isCounting() const
{
	return totalTime < 3.0;
}