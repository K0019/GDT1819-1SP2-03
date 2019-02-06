#include "TreeWinter.h"

// Constructor
TreeWinter::TreeWinter(unsigned int uMatrixMVS, unsigned int uColorData)
	: uMatrixMVS(uMatrixMVS) // UBO handles for model, view matrices and color
	, uColorData(uColorData)
{
	// Generate all the meshes required
	mesh[meshType::PYRAMID] = MeshBuilder::GeneratePyramid(1.0f, 1.0f, uColorData);
	mesh[meshType::CUBE] = MeshBuilder::GenerateCube(1.0f, uColorData);
}

// Destructor
TreeWinter::~TreeWinter()
{
	// Free memory allocated for meshes
	for (int i = 0; i < meshType::NO_OF_MESHES; ++i)
	{
		delete mesh[i];
	}
}

// Render the tree
void TreeWinter::draw(const Mtx44& offset) const
{
	// Set UBO in OpenGL state
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	MS matrixStack; // Initialize matrixStack to offset
	matrixStack.PushMatrix();
	matrixStack.LoadIdentity();
	matrixStack.MultMatrix(offset);

	// Trunk (Cube)
	mesh[meshType::CUBE]->setColor(112, 84, 45, 0.4f, 0.5f, 0.02f, 16);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	matrixStack.PushMatrix(); // 0
	matrixStack.Scale(0.4f, 1.2f, 0.4f);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), matrixStack.Top().a);
	mesh[meshType::CUBE]->Render();
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	matrixStack.PopMatrix(); // 0

	// Leaves (Pyramid, drawLeaves())
	matrixStack.PushMatrix(); // 0
	matrixStack.Translate(0.0f, 1.0f, 0.0f);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), matrixStack.Top().a);
	drawLeaves(matrixStack, 2);
	matrixStack.PopMatrix(); // 0
}

void TreeWinter::drawLeaves(MS& matrixStack, unsigned int count) const
{
	// Set pyramid color to green and draw
	mesh[meshType::PYRAMID]->setColor({ 0.0f, 0.6f, 0.0f }, 0.4f, 0.5f, 0.03f, 16);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), matrixStack.Top().a);
	mesh[meshType::PYRAMID]->Render();
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	
	// Set pyramid color to white and draw with modifications to matrixStack
	mesh[meshType::PYRAMID]->setColor({ 1.0f, 1.0f, 1.0f }, 0.4f, 0.5f, 0.05f, 16);
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	matrixStack.PushMatrix();
	matrixStack.Translate(0.0f, 0.65f, 0.0f);
	matrixStack.Scale(0.75f, 0.75f, 0.75f);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), matrixStack.Top().a);
	mesh[meshType::PYRAMID]->Render();
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);

	// Add more modifications to matrixStack for next leaves draw call
	matrixStack.Translate(0.0f, 0.65f, 0.0f);
	matrixStack.Scale(0.75f, 0.75f, 0.75f);
	if (--count) // If count is not 1, call this function again
	{
		drawLeaves(matrixStack, count);
	}
	// Remove the modifications in matrixStack
	matrixStack.PopMatrix();
}