#include "MeshPlaceable.h"

// Constructor
MeshPlaceable::MeshPlaceable(const std::vector<Vector3>& vertexData, const std::vector<TexCoord>& textureData, const std::vector<GLuint>& indexData, const type shaderType, const GLenum mode, const char* imageLocation, unsigned short lengthX, unsigned short lengthZ)
	: Mesh(vertexData, textureData, indexData, shaderType, mode, imageLocation)
	, lengthX(lengthX)
	, lengthZ(lengthZ)
{
	unsigned int index = 0;
	while (index != indexData.size())
	{
		unsigned int vIndex[3];
		for (int i = 0; i < 3; ++i)
		{
			vIndex[i] = indexData[index++];
		}
		triangles.emplace_back(vertexData[vIndex[0]], vertexData[vIndex[1]], vertexData[vIndex[2]]);
	}
}

// Destructor
MeshPlaceable::~MeshPlaceable()
{

}

// Inherited render function, throws exception if called
void MeshPlaceable::Render() const
{
	throw std::exception("Invalid call");
}

// Select shader and color depending on arguments, then render mesh
void MeshPlaceable::Render(bool placed, bool valid) const
{
	if (placed) // Mesh is placed
	{
		// Select inherent shader type and set pre-render settings
		shader::container.useShader(getShaderType());
		preRenderSetting();
	}
	else // Mesh is not placed
	{
		// Disable writing to depth buffer
		glDepthMask(GL_FALSE);
		// Render using selection shader
		shader::container.useShader(type::SHADER_SELECTION);
		// Input appropriate render color
		Vector3 glowColor = Vector3(!valid * 1.0f, valid * 1.0f, 0.0f);
		glUniform3fv(glGetUniformLocation(shader::container.getID(type::SHADER_SELECTION), "glowColor"), 1, &glowColor.x);
	}
	// Bind VAO
	glBindVertexArray(getVAO());
	// Draw using IBO/EBO
	glDrawElements(getMode(), getIndexSize(), GL_UNSIGNED_INT, (void*)(0));

	if (placed) // Mesh is placed
		// Set post-render settings
		postRenderSetting();
	else // Mesh is not placed
		// Re-enable writing to depth buffer
		glDepthMask(GL_TRUE);
}

// Get length of mesh in x axis, in 2x2unit grid space
unsigned short MeshPlaceable::getX() const
{
	return lengthX;
}
// Get length of mesh in z axis, in 2x2unit grid space
unsigned short MeshPlaceable::getZ() const
{
	return lengthZ;
}