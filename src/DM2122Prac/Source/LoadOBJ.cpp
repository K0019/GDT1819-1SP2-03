#include <iostream>
#include <fstream>
#include <map>

#include "LoadOBJ.h"

bool LoadOBJ(
	const char *file_path, 
	std::vector<Vector3> & out_vertices, 
	std::vector<TexCoord> & out_uvs
)
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Impossible to open" << file_path << ". Are you in the right directory?" << std::endl;
		return false;
	}

	std::vector<GLuint> vertexIndices, uvIndices; // normalIndices
	std::vector<Vector3> temp_vertices;
	std::vector<TexCoord> temp_texCoords;
	//std::vector<Vector3> temp_normals;

	while (!fileStream.eof())
	{
		char buf[256];
		fileStream.getline(buf, 256);
		if (strncmp("v ", buf, 2) == 0)
		{
			Vector3 vertex;
			sscanf_s((buf + 2), "%f%f%f", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strncmp("vt ", buf, 3) == 0)
		{
			TexCoord texCoord;
			sscanf_s((buf + 3), "%f%f", &texCoord.x, &texCoord.y);
			temp_texCoords.push_back(texCoord);
		}
		else if (strncmp("vn ", buf, 3) == 0)
		{
			// Geometry shader generates normals.
			// Might switch to pre-calculated normals as that is faster
		}
		else if (strncmp("f ", buf, 2) == 0)
		{
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];

			int matches = sscanf_s((buf + 2), "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2],
				&vertexIndex[3], &uvIndex[3], &normalIndex[3]);

			if (matches == 9)
			{
				for (int i = 0; i < 3; ++i)
					vertexIndices.push_back(vertexIndex[i]);
				for (int i = 0; i < 3; ++i)
					uvIndices.push_back(uvIndex[i]);
				/*for (int i = 0; i < 3; ++i)
					normalIndices.push_back(normalIndex[i]);*/
			}
			else if (matches == 12)
			{
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[2]);
				vertexIndices.push_back(vertexIndex[3]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[2]);
				uvIndices.push_back(uvIndex[3]);
				/*normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[2]);
				normalIndices.push_back(normalIndex[3]);*/
			}
			else
			{
				std::cout << "Error line: " << buf << std::endl
					<< "File can't be read by parser" << std::endl;
				return false;
			}
		}
	}
	fileStream.close();

	for (unsigned int i = 0; i < vertexIndices.size(); ++i)
	{
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		//unsigned int normalIndex = normalIndices[i];

		out_vertices.push_back(temp_vertices[vertexIndex - 1]);
		out_uvs.push_back(temp_texCoords[uvIndex - 1]);
		//out_normals.push_back(temp_normals[normalIndex - 1]);
	}

	return true;
}

struct PackedVertex{
	Vector3 position;
	TexCoord uv;
	bool operator<(const PackedVertex that) const{
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
	};
};

bool getSimilarVertexIndex_fast( 
	PackedVertex & packed, 
	std::map<PackedVertex, unsigned short> & VertexToOutIndex,
	unsigned short & result
){
	std::map<PackedVertex, unsigned short>::iterator it = VertexToOutIndex.find(packed);
	if(it == VertexToOutIndex.end())
	{
		return false;
	}
	else
	{
		result = it->second;
		return true;
	}
}

void IndexVBO(
	std::vector<Vector3> & in_vertices,
	std::vector<TexCoord> & in_uvs,

	std::vector<GLuint> & out_indices,
	std::vector<Vector3> & out_vertices,
	std::vector<TexCoord>& out_texCoords
)
{
	std::map<PackedVertex,unsigned short> VertexToOutIndex;

	// For each input vertex
	for(unsigned int i = 0; i < in_vertices.size(); ++i) 
	{

		PackedVertex packed = {in_vertices[i], in_uvs[i]};

		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = getSimilarVertexIndex_fast( packed, VertexToOutIndex, index);

		if ( found )
		{ 
			// A similar vertex is already in the VBO, use it instead !
			out_indices.push_back( index );
		}
		else
		{ 
			// If not, it needs to be added in the output data.
			out_vertices.emplace_back(in_vertices[i].x, in_vertices[i].y, in_vertices[i].z);
			out_texCoords.push_back(TexCoord{ in_uvs[i].x, in_uvs[i].y });
			unsigned newindex = (unsigned)out_vertices.size() - 1;
			out_indices.push_back( newindex );
			VertexToOutIndex[ packed ] = newindex;
		}
	}
}