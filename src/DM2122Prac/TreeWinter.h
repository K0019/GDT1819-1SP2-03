#pragma once
#ifndef TREEWINTER_H
#define TREEWINTER_H
#include "Source/ShaderPrograms.h"
#include "Source/MeshBuilder.h"
#include "Vector3.h"
#include "MatrixStack.h"

/* File: TreeWinter.h
   Use: Store and handle 'almost' everything about the winter tree
		Does not handle model matrix of tree. This is handled in Scene1 */

class TreeWinter
{
public:
	explicit TreeWinter(unsigned int uMatrixMVS, unsigned int uColorData); // Constructor
	~TreeWinter(); // Destructor

	void draw(const Mtx44& offset) const; // Render the tree
private:
	void drawLeaves(MS& matrixStack, unsigned int count) const; // Render the leaves of the tree; Recursive

	enum meshType // Store types of meshes used in rendering
	{
		PYRAMID,
		CUBE,
		NO_OF_MESHES
	};

	Mesh* mesh[NO_OF_MESHES]; // Store the meshes

	unsigned int uMatrixMVS, uColorData; // UBO handles
};

#endif // !TREEWINTER_H
