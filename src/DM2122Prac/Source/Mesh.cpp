
#include "Mesh.h"

/******************************************************************************/
/*!
\brief
Default constructor - generate VBO/IBO here

\param meshName - name of mesh
*/
/******************************************************************************/

// Constructor
Mesh::Mesh(const std::vector<Vector3>& vertexData, const std::vector<GLuint>& indexData, const type shaderType, const GLenum mode, const unsigned int uColorData)
	: mode(mode) // Mode of rendering (GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN)
	, shaderType(shaderType) // Shader to use (SHADER_2, SHADER_3, SHADER_AXES, etc.)
	, color({ 0.4f, 0.4f, 0.4f }) // Color data, initialize to white
	, diffuseColor({ 0.5f, 0.5f, 0.5f })
	, specularColor({ 0.05f, 0.05f, 0.05f })
	, shininess(16.0f)
	, textureID(0) // Texture ID
	, uColorData(uColorData) // UBO handle for color
{
	// Use specified shader to set VertexAttribArray
	shader::container.useShader(shaderType);
	// Generate VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate VBO and IBO/EBO
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &colorBuffer);
	glGenBuffers(1, &indexBuffer);

	// Setup VBO for vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vector3), &vertexData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	// Setup IBO/EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint), &indexData[0], GL_STATIC_DRAW);
	indexSize = indexData.size();

	// Unbind from current OpenGL state
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Constructor
Mesh::Mesh(const std::vector<Vector3>& vertexData, const std::vector<TexCoord>& textureData, const std::vector<GLuint>& indexData, const type shaderType, const GLenum mode, const char* imageLocation)
	: mode(mode) // Mode of rendering (GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN)
	, shininess(16.0f)
	, shaderType(shaderType) // Shader to use (SHADER_2, SHADER_3, SHADER_AXES, etc.)
{
	// Use specified shader to set VertexAttribArray
	shader::container.useShader(shaderType);

	// Initialize texture and store texture ID
	textureID = LoadTGA(imageLocation);
	// Generate VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate VBO and IBO/EBO
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &colorBuffer);
	glGenBuffers(1, &indexBuffer);
	glGenBuffers(1, &textureBuffer);

	// Setup VBO for vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vector3), &vertexData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	// Setup VBO for texCoords
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, textureData.size() * sizeof(TexCoord), &textureData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	// Setup IBO/EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint), &indexData[0], GL_STATIC_DRAW);
	indexSize = indexData.size();

	// Unbind from current OpenGL state
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Constructor
Mesh::Mesh(const std::vector<Vector3>& vertexData, const std::vector<Vector3>& colorData, const std::vector<GLuint>& indexData, const type shaderType, const GLenum mode)
	: mode(mode) // Mode of rendering (GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN)
	, shaderType(shaderType) // Shader to use (SHADER_2, SHADER_3, SHADER_AXES, etc.)
	, color({ 0.4f, 0.4f, 0.4f }) // Color data, initialize to white
	, diffuseColor({ 0.5f, 0.5f, 0.5f })
	, specularColor({ 0.05f, 0.05f, 0.05f })
	, shininess(16.0f)
	, textureID(0) // Texture ID
	, uColorData(uColorData) // UBO handle to color uniform block
{
	// Use specified shader to set VertexAttribArray
	shader::container.useShader(shaderType);
	// Generate VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate VBO and IBO/EBO
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &colorBuffer);
	glGenBuffers(1, &indexBuffer);

	// Setup VBO for vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vector3), &vertexData[0], GL_STATIC_DRAW);
	switch (shaderType)
	{
	case type::SHADER_AXES:
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		break;
	default:
		throw std::exception("Default case hit");
	}

	// Setup VBO for colors
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, colorData.size() * sizeof(Vector3), &colorData[0], GL_STATIC_DRAW);
	switch (shaderType)
	{
	case type::SHADER_AXES:
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		break;
	default:
		throw std::exception("Default case hit");
	}

	// Setup IBO/EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint), &indexData[0], GL_STATIC_DRAW);
	indexSize = indexData.size();

	// Unbind from current OpenGL state
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/******************************************************************************/
/*!
\brief
Destructor - delete VBO/IBO here
*/
/******************************************************************************/
Mesh::~Mesh()
{
	glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(1, &colorBuffer);
	glDeleteBuffers(1, &vertexBuffer);
	if (textureID)
	{
		glDeleteBuffers(1, &textureBuffer);
		glDeleteTextures(1, &textureID);
	}
	glDeleteVertexArrays(1, &VAO);
}

/******************************************************************************/
/*!
\brief
OpenGL render code
*/
/******************************************************************************/
// Render the mesh
void Mesh::Render() const
{
	// Bind shader to OpenGL state
	shader::container.useShader(shaderType);
	// Bind VAO
	glBindVertexArray(VAO);
	// Set settings before rendering
	preRenderSetting();
	// Draw using IBO/EBO
	glDrawElements(mode, indexSize, GL_UNSIGNED_INT, (void*)(0));
	// Set settings after rendering
	postRenderSetting();
}

// Set color and material of mesh
void Mesh::setColor(const Vector3& color, float ambientWeight, float diffuseWeight, float specularWeight, unsigned int shininess)
{
	this->color = ambientWeight * color;
	diffuseColor = diffuseWeight * color;
	specularColor = specularWeight * color;
	this->shininess = static_cast<float>(shininess);
}

// Set color and material of mesh
void Mesh::setColor(unsigned short r, unsigned short g, unsigned short b, float ambientWeight, float diffuseWeight, float specularWeight, unsigned int shininess)
{
	setColor({ static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f }, ambientWeight, diffuseWeight, specularWeight, shininess);
}

// Set ambient color property
void Mesh::setAmbient(const Vector3& color)
{
	this->color = color;
}

// Set ambient color property
void Mesh::setAmbient(unsigned short r, unsigned short g, unsigned short b, float ambientWeight)
{
	setAmbient({ static_cast<float>(r) / 255.0f * ambientWeight, static_cast<float>(g) / 255.0f * ambientWeight, static_cast<float>(b) / 255.0f * ambientWeight });
}

// Set diffuse color property
void Mesh::setDiffuse(const Vector3& color)
{
	diffuseColor = color;
}

// Set diffuse color property
void Mesh::setDiffuse(unsigned short r, unsigned short g, unsigned short b, float diffuseWeight)
{
	setDiffuse({ static_cast<float>(r) / 255.0f * diffuseWeight, static_cast<float>(g) / 255.0f * diffuseWeight, static_cast<float>(b) / 255.0f * diffuseWeight });
}

// Set specular and shininess color property
void Mesh::setSpecular(const Vector3& color, unsigned int shininess)
{
	specularColor = color;
	this->shininess = static_cast<float>(shininess);
}

// Set specular and shininess color property
void Mesh::setSpecular(unsigned short r, unsigned short g, unsigned short b, float specularWeight, unsigned int shininess)
{
	setSpecular({ static_cast<float>(r) / 255.0f * specularWeight, static_cast<float>(g) / 255.0f * specularWeight, static_cast<float>(b) / 255.0f * specularWeight }, shininess);
}

// Render settings set before draw call
void Mesh::preRenderSetting() const
{
	switch (shaderType)
	{
	case type::SHADER_2: // SingleColor
		glBindBuffer(GL_UNIFORM_BUFFER, uColorData);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 12, &color.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 16, 12, &diffuseColor.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 32, 12, &specularColor.x);
		glBufferSubData(GL_UNIFORM_BUFFER, 44, 8, &shininess);
		break;
	case type::SHADER_3: // Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glUniform1f(glGetUniformLocation(shader::container.getID(type::SHADER_3), "material.shininess"), shininess);
		break;
	case type::SHADER_TEXT: // Text
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		break;
	}
}

// Render settings set after draw call
void Mesh::postRenderSetting() const
{
	switch (shaderType)
	{
	case type::SHADER_TEXT: // Text
		glEnable(GL_DEPTH_TEST);
		break;
	}
}

// Get draw mode
GLenum Mesh::getMode() const
{
	return mode;
}
// Get type of shader used in rendering
type Mesh::getShaderType() const
{
	return shaderType;
}
// Get VAO
unsigned int Mesh::getVAO() const
{
	return VAO;
}
// Get index buffer size
unsigned int Mesh::getIndexSize() const
{
	return indexSize;
}
// Get textureID
unsigned int Mesh::getTextureID() const
{
	return textureID;
}