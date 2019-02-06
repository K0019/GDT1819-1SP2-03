#pragma once
#ifndef LAMP_H
#define LAMP_H
#include "PointLight.h"
#include "Source/MeshBuilder.h"
#include "Base3DPoly.h"

/* File: Lamp.h
   Use: Store lamp properties */

class Lamp : public Base3DPoly
{
public:
	explicit Lamp(const unsigned int uColorData); // Constructor
	~Lamp(); // Destructor

	PointLight light; // Data to send to OpenGL
private:
};

#endif // !LAMP_H
