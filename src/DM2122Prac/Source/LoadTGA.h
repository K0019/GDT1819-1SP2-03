#ifndef LOAD_TGA_H
#define LOAD_TGA_H
#include <vector>

/* File: LoadTGA.h
   Use: Creates a texture ID from a .tga file
		Framework file, added cube map loading */

GLuint LoadTGA(const char *file_path);
GLuint LoadCubeMap(const char* back, const char* front, const char* top, const char* bottom, const char* left, const char* right); // Load a cube map texture from 6 file paths

#endif