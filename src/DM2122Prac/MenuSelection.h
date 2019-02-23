#ifndef MENUSELECTION_H
#define MENUSELECTION_H
#include "Source/MeshBuilder.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "UsefulFunctions.h"

class MenuSelection
{
public:
	MenuSelection(short noOfButtons);
	~MenuSelection();

	short getSelection() const;
protected:
	void update(double dt, GLFWwindow* window);
	void render(unsigned int selection) const;

	virtual void activated(short selection) = 0;

	MeshText* text;

private:
	Mesh** button;
	short noOfButtons;

	short selection;
	double selectionBounce;
};

#endif // !MENUSELECTION_H
