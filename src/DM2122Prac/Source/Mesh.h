#ifndef MESH_H
#define MESH_H

#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "ShaderPrograms.h"
#include "Vector3.h"
#include "Material.h"
#include "LoadTGA.h"
#include "TexCoord.h"

/******************************************************************************/
/*!
Class Mesh:
\brief	To store VBO (vertex & color buffer) and IBO (index buffer)
*/
/******************************************************************************/

/* File: Mesh.h
   Use: Store mesh and handle rendering, color and texture
		Framework file, heavily modified */

class Mesh
{
public:
	Mesh(const std::vector<Vector3>& vertexData, const std::vector<GLuint>& indexData, const type shaderType, const GLenum mode, const unsigned int uColorData); // Constructor for uniform color
	Mesh(const std::vector<Vector3>& vertexData, const std::vector<TexCoord>& textureData, const std::vector<GLuint>& indexData, const type shaderType, const GLenum mode, const char* imageLocation); // Constructor for textured objects
	Mesh(const std::vector<Vector3>& vertexData, const std::vector<Vector3>& colorData, const std::vector<GLuint>& indexData, const type shaderType, const GLenum mode); // Constructor for individual color
	virtual ~Mesh(); // Destructor
	virtual void Render() const; // Render the mesh

	void setColor(const Vector3& color, float ambientWeight, float diffuseWeight, float specularWeight, unsigned int shininess); // Set all color properties
	void setColor(unsigned short r, unsigned short b, unsigned short g, float ambientWeight, float diffuseWeight, float specularWeight, unsigned int shininess); // Set all color properties
	void setAmbient(const Vector3& color); // Set ambient color property
	void setAmbient(unsigned short r, unsigned short b, unsigned short g, float ambientWeight); // Set ambient color property
	void setDiffuse(const Vector3& color); // Set diffuse color property
	void setDiffuse(unsigned short r, unsigned short b, unsigned short g, float diffuseWeight); // Set diffuse color property
	void setSpecular(const Vector3& color, unsigned int shininess); // Set specular and shininess color property
	void setSpecular(unsigned short r, unsigned short b, unsigned short g, float specularWeight, unsigned int shininess); // Set specular and shininess color property

protected:
	virtual void preRenderSetting() const; // Render settings set before draw call
	virtual void postRenderSetting() const; // Render settings set after draw call

	GLenum getMode() const; // Get draw mode
	type getShaderType() const; // Get type of shader used in rendering
	unsigned int getVAO() const; // Get VAO
	unsigned int getIndexSize() const; // Get index buffer size
	unsigned int getTextureID() const; // Get textureID

private:
	GLenum mode; // Mode of render (GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN)
	type shaderType; // Which shader to use (SHADER_1, SHADER_2, SHADER_AXES, SHADER_LAMP)
	unsigned VAO; // VAO id
	unsigned vertexBuffer; // VBO for vertices id
	unsigned colorBuffer; // VBO for color id
	unsigned indexBuffer; // IBO/EBO
	unsigned int textureBuffer; // VBO for texture id
	unsigned int textureID; // Texture ID within opengl
	unsigned indexSize; // Size of IBO/EBO

	unsigned int uColorData; // UBO handle for color

	Vector3 color, diffuseColor, specularColor; // Store color data
	float shininess; // Store shininess

	Material material; // Material of mesh
};

#endif