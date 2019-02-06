#ifndef MATERIAL_H
#define MATERIAL_H
#include "Vector3.h"

/* File: Material.h
   Use: A copy of the structure used in shaders for materials
		Framework file, lightly modified */

struct Material
{
	Vector3 ambient; // Color of ambient
	Vector3 diffuse; // Color of diffuse
	Vector3 specular; // Color of specular

	unsigned int shininess; // Shininess variable
};

#endif // !MATERIAL_H
