#include "Hotbar.h"

unsigned short Hotbar::selection = 0;
int Hotbar::scrollTimes = 0;

// Static scroll callback function that updates selection
void Hotbar::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0)
		--scrollTimes;
	else if (yoffset < 0)
		++scrollTimes;
}

// Constructor
Hotbar::Hotbar(Mesh* slotBox, Mesh* selectionBox, MeshSprite* icons, unsigned short hotbarSize)
	: size(hotbarSize)
	, slotBox(slotBox)
	, selectionBox(selectionBox)
	, icons(icons)
{
	slots = new unsigned int[size];
	
	for (unsigned short i = 0; i < size; ++i)
	{
		slots[i] = i;
	}
}

// Destructor
Hotbar::~Hotbar()
{
	delete[] slots;
	delete slotBox;
	delete selectionBox;
	delete icons;
}

// Update hotbar according to number of scroll inputs since last frame
void Hotbar::update()
{
	while (scrollTimes < 0) // Scroll left
	{
		scrollLeft();
		++scrollTimes;
	}
	while (scrollTimes > 0) // Scroll right
	{
		scrollRight();
		--scrollTimes;
	}
}

// Scroll selection to the left
void Hotbar::scrollLeft()
{
	if (selection == 0) // Wrap-around
		selection = size - 1;
	else // Scroll left
		--selection;
}

// Scroll selection to the right
void Hotbar::scrollRight()
{
	// Scroll
	++selection;
	if (selection == size) // Wrap-around
		selection = 0;
}

// Get meshPlaceable ID of current hotbar selection
unsigned int Hotbar::querySelection() const
{
	return slots[selection];
}

// Render the hotbar
void Hotbar::print(unsigned int uMatrixMVS, unsigned int uMatrixP, unsigned int screenWidth, unsigned int screenHeight) const
{
	// Disable depth testing (HUD)
	glDisable(GL_DEPTH_TEST);
	// Transformation of each mesh
	MS model;
	model.LoadIdentity();
	// Set projection to an orthogonal representation of the screen dimension
	Mtx44 projection;
	projection.SetToOrtho(-(static_cast<double>(screenWidth) / 80.0), static_cast<double>(screenWidth) / 80.0, 0, static_cast<double>(screenHeight) / 30.0, 0, 0.01);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixP);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), projection.a);

	model.PushMatrix(); // 1

	// Length to translate to the left
	float backTranslate = (size - 1) * -1.0f;
	// Length to translate to the right
	float forwardTranslate = 0.0f;

	// Render each hotbar box
	for (unsigned short count = 0; count < size; ++count)
	{
		model.PushMatrix(); // 2
		model.Translate(backTranslate + forwardTranslate, 1.5f, 0.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
		slotBox->Render();

		model.PopMatrix(); // 2

		forwardTranslate += 2.0f;
	}

	// Render selection box
	forwardTranslate = selection * 2.0f;
	model.PushMatrix(); // 2
	model.Translate(backTranslate + forwardTranslate, 1.5f, 0.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	selectionBox->Render();
	model.PopMatrix(); // 2

	// Start from 2nd hotbar box onwards
	backTranslate = (size - 1) * -1.0f;
	// Reset translation to the right
	forwardTranslate = 0.0f;

	// Render each sprite
	for (unsigned short count = 0; count < size; ++count)
	{
		model.PushMatrix(); // 2
		model.Translate(backTranslate + forwardTranslate, 1.5f, 0.0f);
		model.Scale(1.75f, 1.75f, 1.75f);
		glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
		icons->printSpriteID(slots[count]);
		model.PopMatrix(); // 2

		forwardTranslate += 2.0f;
	}

	model.PopMatrix(); // 1

	// Re-enable depth testing
	glEnable(GL_DEPTH_TEST);
}