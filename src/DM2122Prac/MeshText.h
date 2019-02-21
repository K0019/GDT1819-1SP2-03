#ifndef MESHTEXT_H
#define MESHTEXT_H
#include <algorithm>
#include "Source//Mesh.h"
#include "MatrixStack.h"

/* File: MeshText.h
   Use: Mesh that renders 2D text from a spritesheet */

class MeshText : public Mesh
{
public:
	explicit MeshText(const std::vector<Vector3>& vertexData, const std::vector<TexCoord>& texCoordData, const std::vector<GLuint>& indexData, const char* imageLocation, type shaderType = type::SHADER_TEXT); // Constructor
	virtual ~MeshText(); // Destructor

	void PrintTextForward(const std::string& text, unsigned int uMatrixMVS, float x, float y, float size) const; // Print text from left to right
	void PrintTextBackward(std::string text, unsigned int uMatrixMVS, float x, float y, float size) const; // Print text from right to left

protected:
	void Render(unsigned int offset) const; // Internal render function to render a single character

	virtual void preRenderSetting() const override; // Render settings set before draw call
	virtual void postRenderSetting() const override; // Render settings set after draw call

private:
};

#endif // !MESHTEXT_H
