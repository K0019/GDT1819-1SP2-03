#include "Object.h"

// Constructor
Object::Object(MeshPlaceable* mesh/*unsigned int ID*/, int gridX, int gridZ, Object::Rotation rotation)
	: mesh(mesh)
	, gridX(gridX)
	, gridZ(gridZ)
	, rotation(rotation)
	, gridOccupation(createAABB(mesh, gridX, gridZ, rotation))
{
}

// Destructor
Object::~Object()
{

}

// Render this object
void Object::render(unsigned int uMatrixMVS) const
{
	MS model;
	model.LoadIdentity();

	model.PushMatrix();
	model.Translate(static_cast<float>(gridX << 1), 0.0f, static_cast<float>(gridZ << 1)); // Grid translation
	model.Rotate(rotation * 90.0f, 0.0f, 1.0f, 0.0f); // Orientation
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);
	mesh->Render(true); // Render as placed
	model.PopMatrix();
}

// Retrieve AABB collider for this object
const AABB& Object::getAABB() const
{
	return gridOccupation;
}

// Rotate a rotation clockwise
Object::Rotation& operator++(Object::Rotation& rotation)
{
	if (rotation == Object::Rotation::WEST)
		rotation = Object::Rotation::NORTH;
	else
		rotation = static_cast<Object::Rotation>(static_cast<int>(rotation) + 1);

	return rotation;
}

// Generate an AABB of a meshPlaceable
AABB Object::createAABB(const MeshPlaceable* mesh, int gridX, int gridZ, Object::Rotation rotation)
{
	return createAABB(mesh->getX(), 0, mesh->getZ(), gridX, 0, gridZ, rotation);
}
// Generate a custom AABB
AABB Object::createAABB(int lengthX, int lengthY, int lengthZ, int gridX, int gridY, int gridZ, Object::Rotation rotation)
{
	// Create an AABB aligned with grid space
	switch (rotation)
	{
	case Object::Rotation::NORTH:
	case Object::Rotation::SOUTH:
		return AABB(Vector3(static_cast<float>(gridX - (lengthX >> 1) - (rotation == Object::Rotation::NORTH) * (lengthX % 2)) + 0.1f, 0.0f, static_cast<float>(gridZ - (lengthZ >> 1) - (rotation == Object::Rotation::SOUTH) * (lengthZ % 2)) + 0.1f),
			Vector3(static_cast<float>(gridX + (lengthZ >> 1) - (rotation == Object::Rotation::SOUTH) * (lengthZ % 2)) - 0.1f, 20.0f, static_cast<float>(gridZ + (lengthZ >> 1) - (rotation == Object::Rotation::NORTH) * (lengthZ % 2)) - 0.1f));
	case Object::Rotation::EAST:
	case Object::Rotation::WEST:
		return AABB(Vector3(static_cast<float>(gridX - (lengthZ >> 1) + (rotation == Object::Rotation::EAST) * (lengthZ % 2)) + 0.1f, 0.0f, static_cast<float>(gridZ - (lengthX >> 1) + (rotation == Object::Rotation::EAST) * (lengthX % 2)) + 0.1f),
			Vector3(static_cast<float>(gridX + (lengthZ >> 1) + (rotation == Object::Rotation::WEST) * (lengthZ % 2)) - 0.1f, 20.0f, static_cast<float>(gridZ + (lengthX >> 1) + (rotation == Object::Rotation::WEST) * (lengthX % 2)) - 0.1f));
	default:
		throw std::exception("Control should never reach here");
	}
}

void Object::setID(unsigned int id)
{
	ID = id;
}

int Object::getGridX() const
{
	return gridX;
}

int Object::getGridY() const
{
	return gridY;
}

int Object::getGridZ() const
{
	return gridZ;
}

int Object::getrotation() const
{
	return rotation;
}

int Object::getID() const
{
	return ID;
}

