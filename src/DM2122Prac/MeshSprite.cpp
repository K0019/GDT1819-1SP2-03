#include "MeshSprite.h"

// Constructor
MeshSprite::MeshSprite(const std::vector<Vector3>& vertexData, const std::vector<TexCoord>& texCoordData, const std::vector<GLuint>& indexData, const char* imageLocation)
	: Mesh(vertexData, texCoordData, indexData, type::SHADER_TEXT, GL_TRIANGLES, imageLocation)
{

}

// Destructor
MeshSprite::~MeshSprite()
{
	
}

// Inherited render function, throws exception if called
void MeshSprite::Render() const
{
	throw std::exception("Invalid call");
}

// Renders a sprite according to sprite ID
void MeshSprite::printSpriteID(unsigned int ID) const
{
	// Bind shader to OpenGL state
	shader::container.useShader(getShaderType());
	// Bind VAO
	glBindVertexArray(getVAO());

	// Set pre-render settings
	preRenderSetting();

	// Draw a sprite from a spritesheet
	glDrawElements(getMode(), 6, GL_UNSIGNED_INT, (void*)(ID * 6 * sizeof(GLuint)));

	// Set post-render settings
	postRenderSetting();
}

// Render settings set before draw call
void MeshSprite::preRenderSetting() const
{
	// Disable depth testing (HUD)
	glDisable(GL_DEPTH_TEST);
	// Bind texture to the spritesheet
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, getTextureID());
}

// Render settings set after draw call
void MeshSprite::postRenderSetting() const
{
	// Re-enable depth testing
	glEnable(GL_DEPTH_TEST);
}