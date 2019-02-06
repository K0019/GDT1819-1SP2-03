#include "MeshFloor.h"

// Constructor
MeshFloor::MeshFloor(Mesh* mesh)
	: mesh(mesh)
{

}

// Destructor
MeshFloor::~MeshFloor()
{
	delete mesh;
}

// Render (9 times, at and around the player)
void MeshFloor::render(const Vector3& playerPos, unsigned int uMatrixMVS) const
{
	// Get x and z coordinate extremes of 9 squares around player within a 200x200unit square grid
	int lastX = static_cast<int>(playerPos.x / 200.0f) - 1;
	int nextX = static_cast<int>(playerPos.x / 200.0f) + 1;
	int lastZ = static_cast<int>(playerPos.z / 200.0f) - 1;
	int nextZ = static_cast<int>(playerPos.z / 200.0f) + 1;

	// Render settings
	MS model;
	model.LoadIdentity();
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);

	// Centre
	model.PushMatrix();
	model.Translate((lastX + 1) * 200.0f, 0.0f, (lastZ + 1) * 200.0f);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	mesh->Render();
	model.PopMatrix();
	// Top Left
	model.PushMatrix();
	model.Translate(lastX * 200.0f, 0.0f, nextZ * 200.0f);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	mesh->Render();
	model.PopMatrix();
	// Top
	model.PushMatrix();
	model.Translate((lastX + 1) * 200.0f, 0.0f, nextZ * 200.0f);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	mesh->Render();
	model.PopMatrix();
	// Top Right
	model.PushMatrix();
	model.Translate(nextX * 200.0f, 0.0f, nextZ * 200.0f);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	mesh->Render();
	model.PopMatrix();
	// Right
	model.PushMatrix();
	model.Translate(nextX * 200.0f, 0.0f, (lastZ + 1) * 200.0f);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	mesh->Render();
	model.PopMatrix();
	// Bottom Right
	model.PushMatrix();
	model.Translate(nextX * 200.0f, 0.0f, lastZ * 200.0f);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	mesh->Render();
	model.PopMatrix();
	// Bottom
	model.PushMatrix();
	model.Translate((lastX + 1) * 200.0f, 0.0f, lastZ * 200.0f);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	mesh->Render();
	model.PopMatrix();
	// Bottom Left
	model.PushMatrix();
	model.Translate(lastX * 200.0f, 0.0f, lastZ * 200.0f);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	mesh->Render();
	model.PopMatrix();
	// Left
	model.PushMatrix();
	model.Translate(lastX * 200.0f, 0.0f, (lastZ + 1) * 200.0f);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	mesh->Render();
	model.PopMatrix();
}