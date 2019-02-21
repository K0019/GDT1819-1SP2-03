#include "MeshText.h"

// Constructor
MeshText::MeshText(const std::vector<Vector3>& vertexData, const std::vector<TexCoord>& texCoordData, const std::vector<GLuint>& indexData, const char* imageLocation)
	: Mesh(vertexData, texCoordData, indexData, type::SHADER_TEXT, GL_TRIANGLES, imageLocation)
{

}

// Destructor
MeshText::~MeshText()
{

}

// Render settings set before draw call
void MeshText::preRenderSetting() const
{
	// Disable depth testing (HUD)
	glDisable(GL_DEPTH_TEST);
	// Bind texture to text spritesheet
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, getTextureID());
}

// Render settings set after draw call
void MeshText::postRenderSetting() const
{
	// Re-enable depth testing
	glEnable(GL_DEPTH_TEST);
}

// Print text from left to right
void MeshText::PrintTextForward(const std::string& text, unsigned int uMatrixMVS, float x, float y, float size) const
{
	// Bind shader to OpenGL state
	shader::container.useShader(getShaderType());
	// Bind VAO
	glBindVertexArray(getVAO());

	// Set pre-render settings
	preRenderSetting();
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);

	// Transform text
	MS model;
	model.LoadIdentity();
	model.PushMatrix();
	model.Scale(size, size, size);
	// Offset from left
	float offset = 0.0f;
	
	// Render the text
	for (auto& c : text)
	{
		model.PushMatrix();
		model.Translate(x + static_cast<float>(offset), y, 0.0f);
		offset += 0.7f;
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), &model.Top());
		
		Render(c);
		model.PopMatrix();
	}
	model.PopMatrix();

	// Set post-render settings
	postRenderSetting();
}

// Print text from right to left
void MeshText::PrintTextBackward(std::string text, unsigned int uMatrixMVS, float x, float y, float size) const
{
	// Reverse supplied text
	std::reverse(text.begin(), text.end());
	// Bind shader to OpenGL state
	shader::container.useShader(getShaderType());
	// Bind VAO
	glBindVertexArray(getVAO());

	// Set pre-render settings
	preRenderSetting();
	glBindBuffer(GL_UNIFORM_BUFFER, uMatrixMVS);

	// Transform text
	MS model;
	model.PushMatrix();
	model.Scale(size, size, size);
	// Offset from right
	float offset = 0.0f;

	// Render the text
	for (auto& c : text)
	{
		model.PushMatrix();
		model.Translate(x + static_cast<float>(offset), y, 0.0f);
		offset -= 0.7f;
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mtx44), model.Top().a);

		Render(c);
		model.PopMatrix();
	}
	model.PopMatrix();

	// Set post-render settings
	postRenderSetting();
}

// Internal render function to render a single character
void MeshText::Render(unsigned int asciiValue) const
{
	// Draw using IBO/EBO from a spritesheet
	glDrawElements(getMode(), 6, GL_UNSIGNED_INT, (void*)(asciiValue * 6 * sizeof(GLuint)));
}