#ifndef HOTBAR_H
#define HOTBAR_H
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "MatrixStack.h"
#include "Source/Mesh.h"
#include "MeshSprite.h"

/* File: Hotbar.h
   Use: Class containing information about the hotbar
		Handles hotbar rendering and scroll input */

class Hotbar
{
public:
	Hotbar(Mesh* slotBox, Mesh* selectionBox, MeshSprite* icons, unsigned short size); // Constructor
	~Hotbar(); // Destructor

	void update(); // Update hotbar according to number of scroll inputs since last frame

	unsigned int querySelection() const; // Get meshPlaceable ID of current hotbar selection
	void print(unsigned int uMatrixMVS, unsigned int uMatrixP, unsigned int screenWidth, unsigned int screenHeight) const; // Render the hotbar

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset); // Scroll callback function that updates selection
protected:

private:
	void scrollLeft(); // Scroll selection to the left
	void scrollRight(); // Scroll selection to the right

	Mesh* slotBox, *selectionBox; // Vertical meshes of hotbar boxes
	MeshSprite* icons; // Spritesheet of meshPlaceable icons
	
	unsigned int* slots; // Contains meshPlaceable ID per slot
	unsigned short size; // Hotbar sizd

	static unsigned short selection; // Hotbar selection
	static int scrollTimes; // No. of times scrolled
};

#endif // !HOTBAR_H
