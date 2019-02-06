#ifndef MESHSKYBOX_H
#define MESHSKYBOX_H
#include "Mesh.h"

/* File: MeshSkybox.h
   Use: A mesh that renders a cubemap textured with a skybox texture */

class MeshSkybox : public Mesh
{
public:
	explicit MeshSkybox(const std::vector<Vector3>& vertexData, const std::vector<GLuint>& indexData, 
		const char* back, const char* front, const char* top, const char* bottom, const char* left, const char* right); // Constructor
	virtual ~MeshSkybox(); // Destructor

protected:
	virtual void preRenderSetting() const override; // Render settings set before draw call
	virtual void postRenderSetting() const override; // Render settings set after draw call
private:
	unsigned int cubemap; // Texture ID within opengl
};

#endif // !MESHSKYBOX_H
