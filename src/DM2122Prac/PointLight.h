#pragma once
#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "Vector3.h"

/* File: PointLight.h
   Use: A copy of the structure used in shaders for point lights 
		Used to set point light data in OpenGL */

struct PointLight {
	Vector3 position; // Position of point light in world space

	float constant; // Constant value of attenuation
	float linear; // Linear value of attenuation
	float quadratic; // Quadratic value of attenuation

	Vector3 ambient; // Ambient color
	Vector3 diffuse; // Diffuse color
	Vector3 specular; // Specular color
};

#endif //!POINTLIGHT_H