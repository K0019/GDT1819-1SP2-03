#ifndef SHADER_HPP
#define SHADER_HPP
#include "GL/glew.h"
#include "GLFW/glfw3.h"

/* File: shader.hpp
   Use: Create, load and compile shaders from file 
		Framework file, modified 
		Adapted to include geometry shader */

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path, const char* geometry_file_path);

#endif
