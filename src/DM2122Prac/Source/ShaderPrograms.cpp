#include "ShaderPrograms.h"
#include <stdexcept>
#include <iostream>

// Initialize static variables
short ShaderProg::count = 0;
ShaderProg shader::container;

// Constructor
ShaderProg::ShaderProg()
{
	// Throw exception if attempting to create more than one ShaderProg
	if (count == 1)
		throw std::exception("ShaderProg->Can only create 1 ShaderProg");
	++count;
}

// Destructor
ShaderProg::~ShaderProg()
{
	// Free memory used for each shader
	--count;
	for (int i = 0; i < type::NO_OF_SHADER; ++i)
	{
		glDeleteProgram(shader[i]);
	}
}

// Initialize
void ShaderProg::init()
{
	// Load the shaders from file
	shader[type::SHADER_2] = LoadShaders(
		"Shader//SingleColorVertexShader.vert",
		"Shader//SingleColorFragmentShader.frag",
		"Shader//SingleColorGeometryShader.geog"
	);
	shader[type::SHADER_3] = LoadShaders(
		"Shader//TextureColorVertexShader.vert",
		"Shader//TextureColorFragmentShader.frag",
		"Shader//TextureColorGeometryShader.geog"
	);
	shader[type::SHADER_SELECTION] = LoadShaders(
		"Shader//PlacementVertexShader.vert",
		"Shader//PlacementFragmentShader.frag",
		""
	);
	shader[type::SHADER_AXES] = LoadShaders(
		"Shader//AxesVertexShader.vert",
		"Shader//AxesFragmentShader.frag",
		"Shader//AxesGeometryShader.geog"
	);
	shader[type::SHADER_LAMP] = LoadShaders(
		"Shader//LampVertexShader.vert",
		"Shader//LampFragmentShader.frag",
		"Shader//LampGeometryShader.geog"
	);
	shader[type::SHADER_SKYBOX] = LoadShaders(
		"Shader//SkyboxVertexShader.vert",
		"Shader//SkyboxFragmentShader.frag",
		""
	);
	shader[type::SHADER_TEXT] = LoadShaders(
		"Shader//TextVertexShader.vert",
		"Shader//TextFragmentShader.frag",
		""
	);

	// Set uniform block indexes
	/* 
		0 = MatrixMV
		1 = MatrixP
		2 = ColorData
		3 = uSpotLight
	*/
	setUniformID(type::SHADER_2, getUniformID(type::SHADER_2, "MatrixMV"), 0);
	setUniformID(type::SHADER_2, getUniformID(type::SHADER_2, "MatrixP"), 1);
	setUniformID(type::SHADER_2, getUniformID(type::SHADER_2, "ColorData"), 2);
	setUniformID(type::SHADER_2, getUniformID(type::SHADER_2, "uSpotLight"), 3);
	setUniformID(type::SHADER_3, getUniformID(type::SHADER_3, "MatrixMV"), 0);
	setUniformID(type::SHADER_3, getUniformID(type::SHADER_3, "MatrixP"), 1);
	setUniformID(type::SHADER_3, getUniformID(type::SHADER_3, "uSpotLight"), 3);
	useShader(type::SHADER_3);
	glUniform1i(glGetUniformLocation(getID(type::SHADER_3), "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(getID(type::SHADER_3), "material.specular"), 1);
	setUniformID(type::SHADER_SELECTION, getUniformID(type::SHADER_SELECTION, "MatrixMV"), 0);
	setUniformID(type::SHADER_SELECTION, getUniformID(type::SHADER_SELECTION, "MatrixP"), 1);
	setUniformID(type::SHADER_AXES, getUniformID(type::SHADER_AXES, "MatrixMV"), 0);
	setUniformID(type::SHADER_AXES, getUniformID(type::SHADER_AXES, "MatrixP"), 1);
	setUniformID(type::SHADER_LAMP, getUniformID(type::SHADER_LAMP, "MatrixMV"), 0);
	setUniformID(type::SHADER_LAMP, getUniformID(type::SHADER_LAMP, "MatrixP"), 1);
	setUniformID(type::SHADER_SKYBOX, getUniformID(type::SHADER_SKYBOX, "MatrixMV"), 0);
	setUniformID(type::SHADER_SKYBOX, getUniformID(type::SHADER_SKYBOX, "MatrixP"), 1);
	setUniformID(type::SHADER_TEXT, getUniformID(type::SHADER_TEXT, "MatrixMV"), 0);
	setUniformID(type::SHADER_TEXT, getUniformID(type::SHADER_TEXT, "MatrixP"), 1);
	useShader(type::SHADER_TEXT);
	glUniform1i(glGetUniformLocation(getID(type::SHADER_TEXT), "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(getID(type::SHADER_TEXT), "material.specular"), 1);
}

// Bind the specified shader
void ShaderProg::useShader(type type) const
{
	if (type == type::NO_OF_SHADER)
		throw std::invalid_argument("ShaderProg::useShader->Type must not be NO_OF_SHADER");
	glUseProgram(shader[type]);
}

// Get the specified shader's internal ID in OpenGL
const unsigned int ShaderProg::getID(type type) const
{
	if (type == type::NO_OF_SHADER)
		throw std::invalid_argument("ShaderProg::useShader->Type must not be NO_OF_SHADER");
	return shader[type];
}

// Get the specified uniform block's internal ID in OpenGL
const unsigned int ShaderProg::getUniformID(type type, const char* const name) const
{
	if (type == type::NO_OF_SHADER)
		throw std::invalid_argument("ShaderProg::useShader->Type must not be NO_OF_SHADER");
	return glGetUniformBlockIndex(shader[type], name);
}

// Set the specified uniform block's internal ID in OpenGL
void ShaderProg::setUniformID(type type, unsigned int uniformID, unsigned int newID)
{
	if (type == type::NO_OF_SHADER)
		throw std::invalid_argument("ShaderProg::useShader->Type must not be NO_OF_SHADER");
	glUniformBlockBinding(shader[type], uniformID, newID);
}