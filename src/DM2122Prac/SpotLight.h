#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H
#include "Vector3.h"

/* File: SpotLight.h
   Use: A copy of the structure used in shaders for spotlights
		Used to set spotlight data in OpenGL */

struct SpotLight
{
	Vector3 position; // Position of spotlight in world space

	float constant; // Constant value of attenuation
	float linear; // Linear value of attenuation
	float quadratic; // Quadratic value of attenuation

	Vector3 ambient; // Ambient color
	Vector3 diffuse; // Diffuse color
	Vector3 specular; // Specular color

	Vector3 direction; // Spotlight direction
	float cosInner; // Cosine value of inner lighting angle
	float cosOuter; // Cosine value of outer lighting angle
};

#endif // !SPOTLIGHT_H
