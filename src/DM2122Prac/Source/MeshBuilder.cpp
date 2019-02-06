#include "MeshBuilder.h"
#include <GL\glew.h>

// Create a mesh from an obj and tga file
Mesh* MeshBuilder::GenerateOBJ(const std::string& obj_file_path, const std::string& tex_file_path, type shader)
{
	// Temporary .obj data storage
	std::vector<Vector3> vertices;
	std::vector<TexCoord> uvs;
	
	// Attempt to load .obj
	if (!LoadOBJ(obj_file_path.c_str(), vertices, uvs))
	{
		// Return nullptr if failed to load .obj
		return NULL;
	}

	// Generate proper VBO and EBO/IBO data
	std::vector<Vector3> vertex_buffer_data;
	std::vector<TexCoord> texture_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, index_buffer_data, vertex_buffer_data, texture_buffer_data);

	// Return created mesh
	return new Mesh(vertex_buffer_data, texture_buffer_data, index_buffer_data, shader, GL_TRIANGLES, tex_file_path.c_str());
}

// Create a meshPlaceable from an obj and tga file
MeshPlaceable* MeshBuilder::GenerateMeshPlaceable(const std::string& obj_file_path, const std::string& tex_file_path, type shader, unsigned short lengthX, unsigned short lengthZ)
{
	// Temporary .obj data storage
	std::vector<Vector3> vertices;
	std::vector<TexCoord> uvs;

	// Attempt to load .obj
	if (!LoadOBJ(obj_file_path.c_str(), vertices, uvs))
	{
		// Return nullptr if failed to load .obj
		return NULL;
	}

	// Generate proper VBO and EBO/IBO data
	std::vector<Vector3> vertex_buffer_data;
	std::vector<TexCoord> texture_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, index_buffer_data, vertex_buffer_data, texture_buffer_data);

	// Return created mesh
	return new MeshPlaceable(vertex_buffer_data, texture_buffer_data, index_buffer_data, shader, GL_TRIANGLES, tex_file_path.c_str(), lengthX, lengthZ);
}

// Create a meshText from a tga file
MeshText* MeshBuilder::GenerateText(unsigned int numRow, unsigned int numCol, const char* imageLocation)
{
	// VBO and EBO/IBO
	std::vector<Vector3> vertex_buffer_data;
	std::vector<TexCoord> texture_buffer_data;
	std::vector<GLuint> index_buffer_data;

	// Step length along u and v axis of image
	float width = 1.0f / numCol, height = 1.0f / numRow;

	// Populate VBO and EBO/IBO
	for (unsigned int i = 0; i < numRow; ++i)
	{
		for (unsigned int j = 0; j < numCol; ++j)
		{
			vertex_buffer_data.push_back(Vector3(0.0f, 1.0f, 0.0f));
			vertex_buffer_data.push_back(Vector3(1.0f, 1.0f, 0.0f));
			vertex_buffer_data.push_back(Vector3(0.0f, 0.0f, 0.0f));
			vertex_buffer_data.push_back(Vector3(1.0f, 0.0f, 0.0f));

			texture_buffer_data.push_back({ j * width, 1.0f - i * height });
			texture_buffer_data.push_back({ (j + 1) * width, 1.0f - i * height });
			texture_buffer_data.push_back({ j * width, 1.0f - (i + 1) * height });
			texture_buffer_data.push_back({ (j + 1) * width, 1.0f - (i + 1) * height });

			index_buffer_data.push_back(i * numCol * 4 + j * 4 + 1);
			index_buffer_data.push_back(i * numCol * 4 + j * 4);
			index_buffer_data.push_back(i * numCol * 4 + j * 4 + 2);
			index_buffer_data.push_back(i * numCol * 4 + j * 4 + 1);
			index_buffer_data.push_back(i * numCol * 4 + j * 4 + 2);
			index_buffer_data.push_back(i * numCol * 4 + j * 4 + 3);
		}
	}

	// Return created mesh
	return new MeshText(vertex_buffer_data, texture_buffer_data, index_buffer_data, imageLocation);
}

// Create a meshSprite from a tga file
MeshSprite* MeshBuilder::GenerateSprite(unsigned int numRow, unsigned int numCol, const char* imageLocation)
{
	// VBO and EBO/IBO
	std::vector<Vector3> vertex_buffer_data;
	std::vector<TexCoord> texture_buffer_data;
	std::vector<GLuint> index_buffer_data;

	// Step length along u and v axis of image
	float width = 1.0f / numCol, height = 1.0f / numRow;

	// Populate VBO and EBO/IBO
	for (unsigned int i = 0; i < numRow; ++i)
	{
		for (unsigned int j = 0; j < numCol; ++j)
		{
			vertex_buffer_data.push_back(Vector3(-0.5f, 0.5f, 0.0f));
			vertex_buffer_data.push_back(Vector3(0.5f, 0.5f, 0.0f));
			vertex_buffer_data.push_back(Vector3(-0.5f, -0.5f, 0.0f));
			vertex_buffer_data.push_back(Vector3(0.5f, -0.5f, 0.0f));

			texture_buffer_data.push_back({ j * width, 1.0f - i * height });
			texture_buffer_data.push_back({ (j + 1) * width, 1.0f - i * height });
			texture_buffer_data.push_back({ j * width, 1.0f - (i + 1) * height });
			texture_buffer_data.push_back({ (j + 1) * width, 1.0f - (i + 1) * height });

			index_buffer_data.push_back(i * numCol * 4 + j * 4 + 1);
			index_buffer_data.push_back(i * numCol * 4 + j * 4);
			index_buffer_data.push_back(i * numCol * 4 + j * 4 + 2);
			index_buffer_data.push_back(i * numCol * 4 + j * 4 + 1);
			index_buffer_data.push_back(i * numCol * 4 + j * 4 + 2);
			index_buffer_data.push_back(i * numCol * 4 + j * 4 + 3);
		}
	}

	// Return created mesh
	return new MeshSprite(vertex_buffer_data, texture_buffer_data, index_buffer_data, imageLocation);
}

// Create a meshFloor from a tga file
MeshFloor* MeshBuilder::GenerateGrass(const std::string& tex_file_path, float size, unsigned int repeat)
{
	// Determine length from 0 in x and z direction
	size /= 2.0f;

	// Populate VBO and EBO/IBO
	std::vector<Vector3> vertex_buffer_data
	{
		Vector3(-size, 0.0f, size),
		Vector3(size, 0.0f, size),
		Vector3(-size, 0.0f, -size),
		Vector3(size, 0.0f, -size)
	};
	std::vector<TexCoord> texture_buffer_data
	{
		{ 0.0f, static_cast<float>(repeat) },
		{ static_cast<float>(repeat), static_cast<float>(repeat) },
		{ 0.0f, 0.0f },
		{ static_cast<float>(repeat), 0.0f }
	};
	std::vector<GLuint> index_buffer_data
	{
		1, 2, 0,
		2, 1, 3
	};

	// Return created mesh
	return new MeshFloor(new Mesh(vertex_buffer_data, texture_buffer_data, index_buffer_data, type::SHADER_3, GL_TRIANGLES, tex_file_path.c_str()));
}

// Create a vertical mesh from a tga file
Mesh* MeshBuilder::GenerateXYPlane(const std::string& tex_file_path, float size, unsigned int repeat, type shader)
{
	// Determine length from 0 in x and y direction
	size /= 2.0f;

	// Populate VBO and EBO/IBO
	std::vector<Vector3> vertex_buffer_data
	{
		Vector3(-size, size, 0.0f),
		Vector3(size, size, 0.0f),
		Vector3(-size, -size, 0.0f),
		Vector3(size, -size, 0.0f)
	};
	std::vector<TexCoord> texture_buffer_data
	{
		{ 0.0f, static_cast<float>(repeat) },
		{ static_cast<float>(repeat), static_cast<float>(repeat) },
		{ 0.0f, 0.0f },
		{ static_cast<float>(repeat), 0.0f }
	};
	std::vector<GLuint> index_buffer_data
	{
		1, 0, 2,
		1, 2, 3
	};

	// Return created mesh
	return new Mesh(vertex_buffer_data, texture_buffer_data, index_buffer_data, shader, GL_TRIANGLES, tex_file_path.c_str());
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes()
{
	// Vertices data
	const std::vector<Vector3> vertex_buffer_data = {
	{ 0.0f, 0.0f, 0.0f },
	{ -100.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 100.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, -100.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 100.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, -100.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 100.0f }
	};
	// Colors data
	const std::vector<Vector3> color_buffer_data = {
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 1.0f, 0.0f, 0.0f },
	{ 1.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f }
	};
	// Indexes data
	const std::vector<GLuint> index_buffer_data = {
		0, 1,
		2, 3,
		4, 5,
		6, 7,
		8, 9,
		10, 11
	};

	// Return created mesh
	return new Mesh(vertex_buffer_data, color_buffer_data, index_buffer_data, type::SHADER_AXES, GL_LINES);
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const unsigned int uColorData)
{
	// Vertices data
	std::vector<Vector3> vertex_buffer_data = {
		{ 100.0f, 0.0f, 100.0f },
		{ 100.0f, 0.0f, -100.0f },
		{ -100.0f, 0.0f, 100.0f },
		{ -100.0f, 0.0f, -100.0f }
	};
	// Indexes data
	std::vector<GLuint> index_buffer_data = {
		0, 1, 2,
		1, 3, 2
	};

	// Return created mesh
	return new Mesh(vertex_buffer_data, index_buffer_data, type::SHADER_2, GL_TRIANGLES, uColorData);
}

Mesh* MeshBuilder::GenerateSquare(const unsigned int uColorData)
{
	// Vertices data
	std::vector<Vector3> vertex_buffer_data = {
		{ 1.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, -1.0f },
		{ -1.0f, 0.0f, 1.0f },
		{ -1.0f, 0.0f, -1.0f }
	};
	// Indexes data
	std::vector<GLuint> index_buffer_data = {
		0, 1, 2,
		1, 3, 2
	};

	// Return created mesh
	return new Mesh(vertex_buffer_data, index_buffer_data, type::SHADER_2, GL_TRIANGLES, uColorData);
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(float length, const unsigned int uColorData)
{
	// Vertices data
	std::vector<Vector3> vertex_buffer_data = {
	{ 1.0f, 1.0f, -1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ -1.0f, 1.0f, 1.0f },
	{ -1.0f, 1.0f, -1.0f },

	{ 1.0f, -1.0f, -1.0f },
	{ 1.0f, -1.0f, 1.0f },
	{ -1.0f, -1.0f, 1.0f },
	{ -1.0f, -1.0f, -1.0f }
	};
	// Indexes data
	std::vector<GLuint> index_buffer_data = {
		2, 1, 3,
		1, 0, 3,

		3, 0, 7,
		0, 4, 7,

		0, 1, 4,
		1, 5, 4,

		1, 2, 5,
		2, 6, 5,

		2, 3, 6,
		3, 7, 6,

		6, 7, 5,
		5, 7, 4
	};

	// Multiply vertices by length
	for (auto& v : vertex_buffer_data)
	{
		v *= length;
	}

	// Return created mesh
	return new Mesh(vertex_buffer_data, index_buffer_data, type::SHADER_2, GL_TRIANGLES, uColorData);
}

// Create a meshSkybox from 6 tga files
Mesh* MeshBuilder::GenerateSkybox(const char* back, const char* front, const char* top, const char* bottom, const char* left, const char* right)
{
	// Vertices data
	std::vector<Vector3> vertex_buffer_data = {
		{ 1.0f, 1.0f, -1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ -1.0f, 1.0f, 1.0f },
	{ -1.0f, 1.0f, -1.0f },

	{ 1.0f, -1.0f, -1.0f },
	{ 1.0f, -1.0f, 1.0f },
	{ -1.0f, -1.0f, 1.0f },
	{ -1.0f, -1.0f, -1.0f }
	};
	// Indexes data
	std::vector<GLuint> index_buffer_data = {
		1, 2, 3,
		0, 1, 3,

		0, 3, 7,
		4, 0, 7,

		1, 0, 4,
		5, 1, 4,

		2, 1, 5,
		6, 2, 5,

		3, 2, 6,
		7, 3, 6,

		7, 6, 5,
		7, 5, 4
	};

	// Return created mesh
	return new MeshSkybox(vertex_buffer_data, index_buffer_data, back, front, top, bottom, left, right);
}

Mesh* MeshBuilder::GenerateCircle(float radius, const unsigned int uColorData)
{
	std::vector<Vector3> vertex_data; // Vertices data
	std::vector<GLuint> index_data; // Indexes data

	vertex_data.push_back({ 0.0f, 0.0f, 0.0f }); // Center
	index_data.push_back(0); // Center index

	// Insert points along a circle
	for (int i = 0, j = 1; i <= 360; i += CIRCLE_FIDELITY, ++j)
	{
		vertex_data.emplace_back( radius * cosf(Math::DegreeToRadian(static_cast<float>(i))), radius * sinf(Math::DegreeToRadian(static_cast<float>(i))), 0.0f );
		index_data.push_back(j);
	}

	// Return created mesh
	return new Mesh(vertex_data, index_data, type::SHADER_2, GL_TRIANGLE_FAN, uColorData);
}

Mesh* MeshBuilder::GenerateRing(float innerRadius, float outerRadius, const unsigned int uColorData)
{
	std::vector<Vector3> vertex_data; // Vertices data
	std::vector<GLuint> index_data; // Indexes data

	// Insert points along 2 circles of different sizes
	for (unsigned int hI = 0, index = 0; hI < 360; hI += RING_FIDELITY, ++index)
	{
		vertex_data.emplace_back(innerRadius * sinf(Math::DegreeToRadian(static_cast<float>(hI))), 0.0f, innerRadius * cosf(Math::DegreeToRadian(static_cast<float>(hI))));
		vertex_data.emplace_back(outerRadius * sinf(Math::DegreeToRadian(static_cast<float>(hI))), 0.0f, outerRadius * cosf(Math::DegreeToRadian(static_cast<float>(hI))));

		index_data.push_back(index);
		index_data.push_back(++index);
	}
	index_data.push_back(0);
	index_data.push_back(1);

	// Return created mesh
	return new Mesh(vertex_data, index_data, type::SHADER_2, GL_TRIANGLE_STRIP, uColorData);
}

Mesh* MeshBuilder::GenerateTriangle(const unsigned int uColorData)
{
	// Vertices data
	std::vector<Vector3> vertex_data {
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, 0.0f },
		{ -1.0f, -1.0f, 0.0f }
	};
	// Indexes data
	std::vector<GLuint> index_data{
		0, 1, 2,
		0, 2, 1
	};

	// Return created mesh
	return new Mesh(vertex_data, index_data, type::SHADER_2, GL_TRIANGLE_STRIP, uColorData);
}

Mesh* MeshBuilder::GenerateSphere(float radius, const unsigned int uColorData, bool isLamp)
{
	std::vector<Vector3> vertex_data; // Vertices data
	std::vector<GLuint> index_data; // Indexes data

	// Counter for indexes
	unsigned int nextIndex = 0;

	// Insert pole
	vertex_data.push_back({ 0.0f, radius * cosf(0.0f), 0.0f });
	// Insert vertices and indexes for each splice
	for (int vI = SPHERE_VERTICAL_FIDELITY; vI < 180; vI += SPHERE_VERTICAL_FIDELITY)
	{
		for (int hI = 0; hI < 360; hI += SPHERE_HORIZONTAL_FIDELITY)
		{
			vertex_data.emplace_back( radius * sinf(Math::DegreeToRadian(static_cast<float>(vI))) * cosf(Math::DegreeToRadian(static_cast<float>(hI))),
									  radius * cosf(Math::DegreeToRadian(static_cast<float>(vI))),
									  radius * sinf(Math::DegreeToRadian(static_cast<float>(vI))) * sinf(Math::DegreeToRadian(static_cast<float>(hI))) );

			index_data.push_back(++nextIndex);
			index_data.push_back((1 + nextIndex > 360 / (SPHERE_HORIZONTAL_FIDELITY)) ? (1 + nextIndex - 360 / (SPHERE_HORIZONTAL_FIDELITY)) : (0));
		}
	}

	// Insert other pole
	vertex_data.push_back({ 0.0f, radius * cosf(Math::DegreeToRadian(180.0f)), 0.0f });
	// Wrap up indexes
	for (int hI = SPHERE_HORIZONTAL_FIDELITY; hI < 360; hI += SPHERE_HORIZONTAL_FIDELITY)
	{
		index_data.push_back(vertex_data.size() - 1);
		index_data.push_back(1 + ++nextIndex - 360 / (SPHERE_HORIZONTAL_FIDELITY));
	}

	// Return created mesh
	return new Mesh(vertex_data, index_data, ((isLamp) ? (type::SHADER_LAMP) : (type::SHADER_2)), GL_TRIANGLE_STRIP, uColorData);
}

Mesh* MeshBuilder::GenerateSplicedSphere(float radius, int spliceLevel, const unsigned int uColorData)
{
	std::vector<Vector3> vertex_data; // Vertices data
	std::vector<GLuint> index_data; // Indexes data

	// Counter for indexes
	unsigned int nextIndex = 0;
	
	// Insert vertices for first splice
	for (int hI = 0; hI < 360; hI += SPHERE_HORIZONTAL_FIDELITY)
	{
		vertex_data.emplace_back(radius * sinf(Math::DegreeToRadian(static_cast<float>(spliceLevel * (SPHERE_VERTICAL_FIDELITY)))) * cosf(Math::DegreeToRadian(static_cast<float>(hI))),
			radius * cosf(Math::DegreeToRadian(static_cast<float>(spliceLevel * (SPHERE_VERTICAL_FIDELITY)))),
			radius * sinf(Math::DegreeToRadian(static_cast<float>(spliceLevel * (SPHERE_VERTICAL_FIDELITY)))) * sinf(Math::DegreeToRadian(static_cast<float>(hI))));
		++nextIndex;
	}

	// Insert vertices and indexes for every other splice
	for (int vI = (spliceLevel + 1) * (SPHERE_VERTICAL_FIDELITY); vI <= 180 - spliceLevel * (SPHERE_VERTICAL_FIDELITY); vI += SPHERE_VERTICAL_FIDELITY)
	{
		for (int hI = 0; hI < 360; hI += SPHERE_HORIZONTAL_FIDELITY)
		{
			vertex_data.emplace_back(radius * sinf(Math::DegreeToRadian(static_cast<float>(vI))) * cosf(Math::DegreeToRadian(static_cast<float>(hI))),
				radius * cosf(Math::DegreeToRadian(static_cast<float>(vI))),
				radius * sinf(Math::DegreeToRadian(static_cast<float>(vI))) * sinf(Math::DegreeToRadian(static_cast<float>(hI))));

			index_data.push_back(nextIndex);
			index_data.push_back(++nextIndex - 1 - 360 / (SPHERE_HORIZONTAL_FIDELITY));
		}
		index_data.push_back(nextIndex - 360 / (SPHERE_HORIZONTAL_FIDELITY));
		index_data.push_back(nextIndex - 2 * (360 / (SPHERE_HORIZONTAL_FIDELITY)));
	}

	// Return created mesh
	return new Mesh(vertex_data, index_data, type::SHADER_2, GL_TRIANGLE_STRIP, uColorData);
}

Mesh* MeshBuilder::GenerateHemisphere(float radius, const unsigned int uColorData)
{
	std::vector<Vector3> vertex_data; // Vertices data
	std::vector<GLuint> index_data; // Indexes data

	// Counter for indexes
	unsigned int nextIndex = 0;

	// Add pole (top)
	vertex_data.emplace_back(0.0f, radius * cosf(0.0f), 0.0f);
	// Add vertices and indexes for each splice
	for (int vI = SPHERE_VERTICAL_FIDELITY; vI <= 90; vI += SPHERE_VERTICAL_FIDELITY)
	{
		for (int hI = 0; hI < 360; hI += SPHERE_HORIZONTAL_FIDELITY)
		{
			vertex_data.emplace_back( radius * sinf(Math::DegreeToRadian(static_cast<float>(vI))) * cosf(Math::DegreeToRadian(static_cast<float>(hI))),
									  radius * cosf(Math::DegreeToRadian(static_cast<float>(vI))),
									  radius * sinf(Math::DegreeToRadian(static_cast<float>(vI))) * sinf(Math::DegreeToRadian(static_cast<float>(hI))) );

			index_data.push_back(++nextIndex);
			index_data.push_back((1 + nextIndex > 360 / (SPHERE_HORIZONTAL_FIDELITY)) ? (1 + nextIndex - 360 / (SPHERE_HORIZONTAL_FIDELITY)) : (0));
		}
	}

	// Add middle (center)
	vertex_data.push_back({ 0.0f, radius * cosf(Math::DegreeToRadian(90.0f)), 0.0f });
	// Wrap up indexes
	for (int hI = SPHERE_HORIZONTAL_FIDELITY; hI < 360; hI += SPHERE_HORIZONTAL_FIDELITY)
	{
		index_data.push_back(vertex_data.size() - 1);
		index_data.push_back(1 + ++nextIndex - 360 / (SPHERE_HORIZONTAL_FIDELITY));
	}

	// Return created mesh
	return new Mesh(vertex_data, index_data, type::SHADER_2, GL_TRIANGLE_STRIP, uColorData);
}

Mesh* MeshBuilder::GenerateCylinder(float radius, const unsigned int uColorData)
{
	std::vector<Vector3> vertex_data; // Vertices data
	std::vector<GLuint> index_data; // Indexes data

	// Counter for indexes
	unsigned int nextIndex = 0;

	// Insert center (top)
	vertex_data.emplace_back(0.0f, radius * cosf(0.0f), 0.0f);
	// Insert vertices and indexes for 2 circles at top and bottom
	for (int vI = 0; vI <= 180; vI += 180)
	{
		for (int hI = 0; hI < 360; hI += CYLINDER_FIDELITY)
		{
			vertex_data.emplace_back(radius * cosf(Math::DegreeToRadian(static_cast<float>(hI))),
									 radius * cosf(Math::DegreeToRadian(static_cast<float>(vI))),
								   	 radius * sinf(Math::DegreeToRadian(static_cast<float>(hI))));

			index_data.push_back(++nextIndex);
			index_data.push_back((1 + nextIndex > 360 / (CYLINDER_FIDELITY)) ? (1 + nextIndex - 360 / (CYLINDER_FIDELITY)) : (0));
		}
	}

	// Insert center (bottom)
	vertex_data.emplace_back(0.0f, -1.0f, 0.0f);
	// Wrap up indexes
	for (int hI = CYLINDER_FIDELITY; hI < 360; hI += CYLINDER_FIDELITY)
	{
		index_data.push_back(vertex_data.size() - 1);
		index_data.push_back(1 + ++nextIndex - 360 / (CYLINDER_FIDELITY));
	}

	// Return created mesh
	return new Mesh(vertex_data, index_data, type::SHADER_2, GL_TRIANGLE_STRIP, uColorData);
}

Mesh* MeshBuilder::GenerateQuartersphere(float radius, const unsigned int uColorData)
{
	std::vector<Vector3> vertex_data; // Vertices data
	std::vector<GLuint> index_data; // Indexes data

	// Counter for indexes
	unsigned int nextIndex = 0;

	// Insert middle (center)
	vertex_data.emplace_back(0.0f, 0.0f, 0.0f);
	// Insert vertices and indexes for each splice
	for (int vI = 0; vI <= 90; vI += QUARTERSPHERE_VERTICAL_FIDELITY)
	{
		for (int hI = 0; hI <= 180; hI += QUARTERSPHERE_HORIZONTAL_FIDELITY)
		{
			vertex_data.emplace_back(radius * cosf(Math::DegreeToRadian(static_cast<float>(vI))) * cosf(Math::DegreeToRadian(static_cast<float>(hI))),
									 radius * sinf(Math::DegreeToRadian(static_cast<float>(vI))),
								   	 radius * cosf(Math::DegreeToRadian(static_cast<float>(vI))) * sinf(Math::DegreeToRadian(static_cast<float>(hI))));
			
			index_data.push_back((nextIndex > 180 / (QUARTERSPHERE_HORIZONTAL_FIDELITY)) ? (nextIndex - 180 / (QUARTERSPHERE_HORIZONTAL_FIDELITY)) : (0));
			index_data.push_back(++nextIndex);
		}
	}

	// Return created mesh
	return new Mesh(vertex_data, index_data, type::SHADER_2, GL_TRIANGLE_STRIP, uColorData);
}

Mesh* MeshBuilder::GeneratePyramid(float length, float height, const unsigned int uColorData)
{
	// Vertices data
	std::vector<Vector3> vertex_buffer_data = {
		{ 1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, 1.0f },
		{ -1.0f, -1.0f, 1.0f },
		{ -1.0f, -1.0f, -1.0f },

		{ 0.0f, 1.0f, 0.0f }
	};
	// Indexes data
	std::vector<GLuint> index_buffer_data = {
		0, 1, 2,
		0, 2, 3,

		0, 4, 1,
		1, 4, 2,
		2, 4, 3,
		3, 4, 0
	};

	// Modify vertices data by scale specified
	for (unsigned int i = 0, size = vertex_buffer_data.size(); i < size; ++i)
	{
		if (i == size - 1)
		{
			vertex_buffer_data[i] *= height;
		}
		else
		{
			vertex_buffer_data[i].x *= length;
			vertex_buffer_data[i].y *= height;
			vertex_buffer_data[i].z *= length;
		}
	}

	// Return created mesh
	return new Mesh(vertex_buffer_data, index_buffer_data, type::SHADER_2, GL_TRIANGLES, uColorData);
}

Mesh* MeshBuilder::GenerateCone(float radius, const unsigned int uColorData)
{
	std::vector<Vector3> vertex_data; // Vertices data
	std::vector<GLuint> index_data; // Indexes data

	// Count for indexes
	unsigned int nextIndex = 0;

	// Insert peak
	vertex_data.emplace_back(0.0f, radius * cosf(0.0f), 0.0f);
	// Insert vertices and indexes for each splice
	for (int hI = 0; hI < 360; hI += CYLINDER_FIDELITY)
	{
		vertex_data.emplace_back(radius * cosf(Math::DegreeToRadian(static_cast<float>(hI))),
								 radius * cosf(Math::DegreeToRadian(180.0f)),
								 radius * sinf(Math::DegreeToRadian(static_cast<float>(hI))));

		index_data.push_back(++nextIndex);
		index_data.push_back((1 + nextIndex > 360 / (CYLINDER_FIDELITY)) ? (1 + nextIndex - 360 / (CYLINDER_FIDELITY)) : (0));
	}

	// Insert middle (bottom)
	vertex_data.emplace_back(0.0f, -1.0f, 0.0f);
	// Wrap up indexes
	for (int hI = CYLINDER_FIDELITY; hI < 360; hI += CYLINDER_FIDELITY)
	{
		index_data.push_back(vertex_data.size() - 1);
		index_data.push_back(1 + ++nextIndex - 360 / (CYLINDER_FIDELITY));
	}

	// Return created mesh
	return new Mesh(vertex_data, index_data, type::SHADER_2, GL_TRIANGLE_STRIP, uColorData);
}

Mesh* MeshBuilder::GenerateTorus(float innerRadius, float outerRadius, const unsigned int uColorData)
{
	std::vector<Vector3> vertex_data; // Vertices data
	std::vector<GLuint> index_data; // Indexes data
	Mtx44 rotate; // Matrix used to rotate vertices

	// Count for indexes
	unsigned int nextIndex = 0;

	// Insert vertices for first splice
	for (unsigned int circleRotate = 0; circleRotate < 360; circleRotate += RING_FIDELITY)
	{
		Vector3 position = { 0.0f,
							 0.5f * (outerRadius - innerRadius) * cosf(Math::DegreeToRadian(static_cast<float>(circleRotate))) + static_cast<float>(innerRadius + 1.0f),
							 0.5f * (outerRadius - innerRadius) * sinf(Math::DegreeToRadian(static_cast<float>(circleRotate))) + static_cast<float>(innerRadius + 1.0f) };
		vertex_data.push_back(position);
		++nextIndex;
	}
	--nextIndex;

	// Insert vertices and indexes for every other splice
	for (unsigned int overallRotate = RING_FIDELITY; overallRotate < 360; overallRotate += RING_FIDELITY)
	{
		rotate.SetToRotation(static_cast<float>(overallRotate), 0.0f, 1.0f, 0.0f);

		for (unsigned int circleRotate = 0; circleRotate < 360; circleRotate += RING_FIDELITY)
		{
			Vector3 position = { 0.0f,
								 0.5f * (outerRadius - innerRadius) * cosf(Math::DegreeToRadian(static_cast<float>(circleRotate))) + static_cast<float>(innerRadius + 1.0f),
								 0.5f * (outerRadius - innerRadius) * sinf(Math::DegreeToRadian(static_cast<float>(circleRotate))) + static_cast<float>(innerRadius + 1.0f) };
			vertex_data.push_back(rotate * position);
			index_data.push_back(1 + nextIndex - 360 / (RING_FIDELITY));
			index_data.push_back(++nextIndex);
		}
		index_data.push_back(1 + nextIndex - 2 * (360 / (RING_FIDELITY)));
		index_data.push_back(1 + nextIndex - 360 / (RING_FIDELITY));
	}

	// Wrap up indexes
	unsigned int initialIndex = 0;
	for (unsigned int circleRotate = 0; circleRotate < 360; circleRotate += RING_FIDELITY)
	{
		index_data.push_back(++nextIndex - 1 * 360 / (RING_FIDELITY));
		index_data.push_back(initialIndex++);
	}
	index_data.push_back(++nextIndex - 2 * 360 / (RING_FIDELITY));
	index_data.push_back(0);

	// Return created mesh
	return new Mesh(vertex_data, index_data, type::SHADER_2, GL_TRIANGLE_STRIP, uColorData);
}


Mesh* MeshBuilder::GenerateTiltedFrustum(const unsigned int uColorData)
{
	Mtx44 rotate; // Matrix used to rotate vertices
	rotate.SetToRotation(10.0f, 1.0f, 0.0f, 0.0f);

	// Vertices data
	std::vector<Vector3> vertex_data {
		rotate * Vector3(0.7f, 1.0f, 0.7f),
		rotate * Vector3(0.7f, 1.0f, -0.7f),
		rotate * Vector3(-0.7f, 1.0f, 0.7f),
		rotate * Vector3(-0.7f, 1.0f, -0.7f),

		{ 1.0f, -0.0f, 1.0f },
		{ 1.0f, -0.0f, -1.0f },
		{ -1.0f, -0.0f, 1.0f },
		{ -1.0f, -0.0f, -1.0f },
	};
	// Indexes data 
	std::vector<GLuint> index_data {
		// Top
		0, 1, 2,
		1, 3, 2,
		// Bottom
		4, 6, 5,
		5, 6, 7,
		// Side 1
		0, 4, 1,
		4, 5, 1,
		// Side 2
		1, 5, 3,
		5, 7, 3,
		// Side 3
		2, 3, 6,
		6, 3, 7,
		// Side 4
		2, 6, 0,
		6, 4, 0
	};

	// Return created mesh
	return new Mesh(vertex_data, index_data, type::SHADER_2, GL_TRIANGLES, uColorData);
}

Mesh* MeshBuilder::GenerateTiltedSphericalFrustum(const unsigned int uColorData)
{
	std::vector<Vector3> vertex_data; // Vertices data
	std::vector<GLuint> index_data; // Indexes data

	// Count for indexes
	unsigned int nextIndex = 0;

	// Matrix used to rotate vertices
	Mtx44 rotate;
	rotate.SetToRotation(10.0f, 1.0f, 0.0f, 0.0f);

	// Insert center (bottom)
	vertex_data.push_back(rotate * Vector3(0.0f, 0.2f, 0.0f));

	// Insert vertices and indexes for bottom circle
	for (int hI = 0; hI < 360; hI += CYLINDER_FIDELITY)
	{
		vertex_data.push_back(rotate * Vector3(0.9f * cosf(Math::DegreeToRadian(static_cast<float>(hI))),
											   0.2f,
											   0.9f * sinf(Math::DegreeToRadian(static_cast<float>(hI)))));

		index_data.push_back(++nextIndex);
		index_data.push_back((1 + nextIndex > 360 / (CYLINDER_FIDELITY)) ? (1 + nextIndex - 360 / (CYLINDER_FIDELITY)) : (0));
	}

	// Insert vertices and indexes for top circle
	for (int hI = 0; hI < 360; hI += CYLINDER_FIDELITY)
	{
		vertex_data.emplace_back(cosf(Math::DegreeToRadian(static_cast<float>(hI))),
									0.0f,
									sinf(Math::DegreeToRadian(static_cast<float>(hI))));

		index_data.push_back(++nextIndex);
		index_data.push_back((1 + nextIndex > 360 / (CYLINDER_FIDELITY)) ? (1 + nextIndex - 360 / (CYLINDER_FIDELITY)) : (0));
	}
	// Insert center (top)
	vertex_data.emplace_back(0.0f, 0.0f, 0.0f);

	// Wrap up indexes
	for (int hI = CYLINDER_FIDELITY; hI < 360; hI += CYLINDER_FIDELITY)
	{
		index_data.push_back(vertex_data.size() - 1);
		index_data.push_back(1 + ++nextIndex - 360 / (CYLINDER_FIDELITY));
	}

	// Return created mesh
	return new Mesh(vertex_data, index_data, type::SHADER_2, GL_TRIANGLE_STRIP, uColorData);
}
