#ifndef MESHPLACEABLE_H
#define MESHPLACEABLE_H
#include <stdexcept>
#include "Source/Mesh.h"

/* File: MeshPlaceable.h
   Use: Mesh that handles rendering of meshes within a grid */

class MeshPlaceable : public Mesh
{
public:
	MeshPlaceable(const std::vector<Vector3>& vertexData, const std::vector<TexCoord>& textureData, const std::vector<GLuint>& indexData,
				  const type shaderType, const GLenum mode, const char* imageLocation, unsigned short lengthX, unsigned short lengthZ); // Constructor
	virtual ~MeshPlaceable(); // Destructor

	void Render(bool placed, bool valid = false) const; // Select shader and color depending on arguments, then render mesh

	unsigned short getX() const; // Get length of mesh in x axis, in 2x2unit grid space
	unsigned short getZ() const; // Get length of mesh in z axis, in 2x2unit grid space
protected:

private:
	void Render() const override; // Inherited render function, throws exception if called

	unsigned short lengthX, lengthZ; // Length of mesh in x and z axis, in 2x2unit grid space
};

#endif // !MESHPLACEABLE_H
