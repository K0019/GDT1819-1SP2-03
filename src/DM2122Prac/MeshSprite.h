#ifndef MESHSPRITE_H
#define MESHSPRITE_H
#include "Source/Mesh.h"

/* File: MeshSprite.h
   Use: Mesh that renders a 2D sprite from a spritesheet */

class MeshSprite : public Mesh
{
public:
	explicit MeshSprite(const std::vector<Vector3>& vertexData, const std::vector<TexCoord>& texCoordData, const std::vector<GLuint>& indexData, const char* imageLocation); // Constructor
	virtual ~MeshSprite(); // Destructor

	void printSpriteID(unsigned int ID) const; // Renders a sprite according to sprite ID
protected:
	virtual void Render() const override; // Inherited render function, throws exception if called

	virtual void preRenderSetting() const override; // Render settings set before draw call
	virtual void postRenderSetting() const override; // Render settings set after draw call

private:

};

#endif // !MESHSPRITE_H
