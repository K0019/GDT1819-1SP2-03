#include "MeshSkybox.h"

// Constructor
MeshSkybox::MeshSkybox(const std::vector<Vector3>& vertexData, const std::vector<GLuint>& indexData, 
	const char* back, const char* front, const char* top, const char* bottom, const char* left, const char* right)
	: Mesh(vertexData, indexData, type::SHADER_SKYBOX, GL_TRIANGLES, NULL)
	, cubemap(LoadCubeMap(back, front, top, bottom, left, right)) // Load cubemap
{
}

// Destructor
MeshSkybox::~MeshSkybox()
{
	glDeleteTextures(1, &cubemap);
}

// Render settings set before draw call
void MeshSkybox::preRenderSetting() const
{
	// Disable depth mask, so that the skybox won't render over everything
	glDepthMask(GL_FALSE);
	// Bind the cubemap texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
}

// Render settings set after draw call
void MeshSkybox::postRenderSetting() const
{
	// Re-enable depth mask
	glDepthMask(GL_TRUE);
}