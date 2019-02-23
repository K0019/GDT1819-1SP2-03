#ifndef COUNTDOWN_H
#define COUNTDOWN_H
#include "Source/Mesh.h"
#include "timer.h"
#include "MatrixStack.h"
#include "Source/ShaderPrograms.h"

class Countdown
{
public:
	Countdown(Mesh* three, Mesh* two, Mesh* one, Mesh* go);
	~Countdown();

	void start();
	void reset();

	void render(unsigned int uMatrixMVS);
	bool isCounting() const;
protected:

private:
	Mesh** meshes;
	StopWatch* timer;

	double totalTime;
};

#endif // !COUNTDOWN_H
