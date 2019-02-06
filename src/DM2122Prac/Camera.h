#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include "MyMath.h"
#include "Vector3.h"

/* File: Camera.h
   Use: Stores information about camera. Does not handle calculations. */

class Camera
{
public:
	Camera(); // Constructor

	void init(); // Sets default values for camera

	void setPos(float x, float y, float z); // Set camera position
	void setPos(const Vector3& position); // Set camera position
	void setFront(float x, float y, float z); // Set camera front

	Vector3 pos, front, up; // Camera attributes

	static bool firstMouse; // Check if it is the first time moving the mouse, avoid sudden camera movement at start of program
	static float mouseX, mouseY; // Track cursor position in window
	float yaw, pitch; // Camera yaw and pitch
};

#endif // !CAMERA_H
