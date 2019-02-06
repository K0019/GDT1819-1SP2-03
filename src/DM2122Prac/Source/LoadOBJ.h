#ifndef LOAD_OBJ_H
#define LOAD_OBJ_H

#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "TexCoord.h"
#include "Vector3.h"

/* File: LoadOBJ.h
   Use: Creates VBO and EBO/IBO from a .obj file
		Framework file, lightly modified
		Removed normals */

bool LoadOBJ(
	const char *file_path, 
	std::vector<Vector3> & out_vertices, 
	std::vector<TexCoord> & out_uvs
);

void IndexVBO(
	std::vector<Vector3> & in_vertices,
	std::vector<TexCoord> & in_uvs,

	std::vector<GLuint> & out_indices,
	std::vector<Vector3> & out_vertices,
	std::vector<TexCoord>& out_texCoords
);

#endif