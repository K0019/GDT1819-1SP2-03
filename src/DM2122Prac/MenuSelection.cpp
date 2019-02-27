#include "MenuSelection.h"

MenuSelection::MenuSelection(short noOfButtons)
	: button(new Mesh*[2])
	, noOfButtons(noOfButtons)
	, selection(0)
	, selectionBounce(-1.0)
{
	button[0] = MeshBuilder::GenerateXYPlane("Image//Red.tga", 10.0f, 8.0f, 1, type::SHADER_TEXT);
	button[1] = MeshBuilder::GenerateXYPlane("Image//Blue.tga", 10.0f, 8.0f, 1, type::SHADER_TEXT);

	text = MeshBuilder::GenerateText(16, 16, "Image//calibri.tga");
}

MenuSelection::~MenuSelection()
{
	for (int i = 0; i < 2; ++i)
	{
		delete button[i];
	}

	delete[] button;
	delete text;
}

void MenuSelection::update(double dt, GLFWwindow* window)
{
	selectionBounce -= dt;

	if (isPressed(window, GLFW_KEY_UP))
	{
		if (selectionBounce <= 0.0)
		{
			if (selection) // Not 0
			{
				--selection;
				selectionBounce = 0.2;
			}
		}
	}
	else if (isPressed(window, GLFW_KEY_DOWN))
	{
		if (selectionBounce <= 0.0)
		{
			if (selection != noOfButtons - 1) // Not last button
			{
				++selection;
				selectionBounce = 0.2;
			}
		}
	}
	else
	{
		selectionBounce = -1.0;
	}
	if (!music::player.isplaying(music::player.getSelectionSound()))
	{

		music::player.playsound(music::player.getSelectionSound());
	}
	if (isPressed(window, GLFW_KEY_ENTER))
	{
		activated(selection);
	}
}

void MenuSelection::render(unsigned int selection) const
{
	if (selection == this->selection)
	{
		button[0]->Render();
	}
	else
	{
		button[1]->Render();
	}
}

short MenuSelection::getSelection() const
{
	return selection;
}