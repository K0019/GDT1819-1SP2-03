#ifndef SHADERPROGRAMS_H
#define SHADERPROGRAMS_H
#include "shader.hpp"
#include <memory>

/* File: ShaderPrograms.h
   Use: Container of all shader programs created by LoadShader 
		Offers simple and easy access to the different shaders */

enum type // Types of shaders available
{
	// SHADER_1 was originally for SimpleXXXXShader.XXXX
	// Removed because it eventually became obsolete within my code
	SHADER_2 = 0, // Normal shader with lighting but without texture
				  // SingleColorXXXXShader.XXXX
	SHADER_3,	 // Normal shader with texture and lighting
				 // TextureColorXXXXShader.XXXX
	SHADER_SELECTION, // Shader that renders meshes in a specified color
					  // PlacementXXXXShader.XXXX
	SHADER_AXES, // Shader that draws lines with specified color, and ignores lighting
				 // AxesXXXXShader.XXXX
	SHADER_LAMP, // Normal shader without lighting
				 // LampXXXXShader.XXXX
	SHADER_SKYBOX, // Shader specifically for skybox
				   // SkyboxXXXXShader.XXXX
	SHADER_TEXT, // Shader that renders meshes in 2D (no modification to view matrix needed)
				 // TextXXXXShader.XXXX
	SHADER_WINLOSE, // Shader that renders meshes in 2D with modifiable alpha
					// TextVertexShader.vert, WinLoseFragmentShader.frag
	NO_OF_SHADER // To be used internally, does not represent any shader
};
class ShaderProg
{
public:
	ShaderProg(); // Constructor
	~ShaderProg(); // Destructor
	void useShader(type type) const; // Bind the specified shader
	const unsigned int getID(type type) const; // Get the specified shader's internal ID in OpenGL
	const unsigned int getUniformID(type type, const char* const name) const; // Get the specified uniform block's internal ID in OpenGL
	void init(); // Initialize
private:
	void setUniformID(type type, unsigned int uniformID, unsigned int newID); // Set the specified uniform block's internal ID in OpenGL
	unsigned int shader[type::NO_OF_SHADER]; // Container for shader IDs

	static short count; // Counter of how many ShaderProg have been created, used to disallow multiple creations of ShaderProg
};

// Public namespace which contains one ShaderProg created in ShaderPrograms.cpp
// Any file which requires use of ShaderProg will reference this ShaderProg object
namespace shader
{
	extern ShaderProg container;
}


#endif // !SHADERPROGRAMS_H
