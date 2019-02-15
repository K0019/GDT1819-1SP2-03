#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include <string>
#include <vector>
#include "Mesh.h"
#include "MeshPlaceable.h"
#include "MeshSkybox.h"
#include "MeshText.h"
#include "MeshSprite.h"
#include "MeshFloor.h"
#include "MyMath.h"
#include "Definitions.h"
#include "Mtx44.h"
#include "LoadOBJ.h"

/******************************************************************************/
/*!
Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/

/* File: MeshBuilder.h
   Use: Handle generating different types of meshes
		Framework file */

class MeshBuilder
{
public:
	static Mesh* GenerateOBJ(const std::string& obj_file_path, const std::string& tex_file_path, type shader); // Create a mesh from an obj and tga file
	static MeshPlaceable* GenerateMeshPlaceable(const std::string& obj_file_path, const std::string& tex_file_path, type shader, unsigned short lengthX, unsigned short lengthZ, bool physicsEnabled); // Create a meshPlaceable from an obj and tga file
	static Mesh* GenerateSkybox(const char* right, const char* left, const char* top, const char* bottom, const char* front, const char* back); // Create a meshSkybox from 6 tga files
	static MeshText* GenerateText(unsigned int numRow, unsigned int numCol, const char* imageLocation); // Create a meshText from a tga file
	static MeshSprite* GenerateSprite(unsigned int numRow, unsigned int numCol, const char* imageLocation); // Create a meshSprite from a tga file
	static MeshFloor* GenerateGrass(const std::string& tex_file_path, float size, unsigned int repeat); // Create a meshFloor from a tga file
	static Mesh* GenerateXYPlane(const std::string& tex_file_path, float size, unsigned int repeat, type shader); // Create a vertical mesh from a tga file

	// The function names literally spell out what they do
	// They allocate memory for a mesh of the function's shape
	// I hope I do not have to comment about what kind of mesh is created for every single function
	static Mesh* GenerateAxes();
	static Mesh* GenerateQuad(const unsigned int uColorData);
	static Mesh* GenerateSquare(const unsigned int uColorData);
	static Mesh* GenerateCircle(float radius, const unsigned int uColorData);
	static Mesh* GenerateRing(float innerRadius, float outerRadius, const unsigned int uColorData);
	static Mesh* GenerateTriangle(const unsigned int uColorData);

	static Mesh* GenerateCube(float length, const unsigned int uColorData);
	static Mesh* GenerateSphere(float radius, const unsigned int uColorData, bool isLamp);
	static Mesh* GenerateSplicedSphere(float radius, int spliceLevel, const unsigned int uColorData);
	static Mesh* GenerateHemisphere(float radius, const unsigned int uColorData);
	static Mesh* GenerateCylinder(float radius, const unsigned int uColorData);
	static Mesh* GenerateQuartersphere(float radius, const unsigned int uColorData);
	static Mesh* GeneratePyramid(float length, float height, const unsigned int uColorData);
	static Mesh* GenerateCone(float radius, const unsigned int uColorData);
	static Mesh* GenerateTorus(float innerRadius, float outerRadius, const unsigned int uColorData);

	// Frustums with their top plane at a 10 degree angle to the bottom plane
	static Mesh* GenerateTiltedFrustum(const unsigned int uColorData);
	static Mesh* GenerateTiltedSphericalFrustum(const unsigned int uColorData);
};

#endif