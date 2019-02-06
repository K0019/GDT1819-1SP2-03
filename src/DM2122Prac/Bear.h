#pragma once
#ifndef BEAR_H
#define BEAR_H
#include "Source/ShaderPrograms.h"
#include "Source/MeshBuilder.h"
#include "Vector3.h"
#include "MatrixStack.h"
#include "timer.h"

/* File: Bear.h
   Use: Store and handle everything about the bear */

class Bear
{
public:
	explicit Bear(unsigned int uMatrixMVS, unsigned int uColorData); // Constructor
	~Bear(); // Destructor

	enum animationState // UNUSED - Animation state
	{
		NONE,
		WAVING,
		WALKING,
		PLAYING
	};

	void update(const Vector3& direction); // UNUSED - Update function; direction is for the bear's walking direction
	bool changeState(animationState state); // UNUSED - Change animation state
	void draw(const Vector3& position, float yaw) const; // Render the bear at specified location and rotation

	animationState getState() const; // Get current animation state
private:
	enum meshType // Different meshes used in the rendering of the bear
	{
		SPHERE = 0,
		CYLINDER,
		HEMISPHERE,
		QUARTERSPHERE,
		SPLICEDSPHERE6,
		SQUARE,
		TILTED_SPHERICAL_FRUSTUM,
		CONE,
		TRIANGLE,
		TORUS,
		NO_OF_MESHES
	};


	void drawLeg(MS& matrixStack, bool right, const Mtx44& legRotation) const; // Draw the leg
	void drawArm(MS& matrixStack, bool right, const Mtx44& armRotation) const; // Draw the arm
	void drawScarf(MS& matrixStack) const; // Draw the scarf
	void drawHead(MS& matrixStack) const; // Draw the head and hat

	void drawHat_NoPop(MS& matrixStack, int count) const; // Draw the middle of the hat, used in drawHead; recursive

	void renderMesh(MS* const matrixStack, Mesh* const) const; // Function containing process of rendering a mesh within draw

	Mesh* mesh[NO_OF_MESHES]; // Container of meshes

	unsigned int uMatrixMVS, uColorData; // UBO handles

	animationState state; // Store animation state
	StopWatch* timer; // Timer to keep track of animation

	double totalTime; // Store total time counted since timer started
};

#endif // !BEAR_H
