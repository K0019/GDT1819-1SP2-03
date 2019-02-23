#version 330 core

/* File: TextVertexShader.vert
   Use: Handle conversion of a 2D mesh in model space to view space 
		Only applicable to 2D sprites */

// No. of point lights and spotlights
#define NO_OF_POINTLIGHTS 0

// Info about primitive
layout (location = 2) in vec3 vertexPosition_modelspace;
layout(location = 5) in vec2 inTexCoord;

// Texture coordinates to sample from texture
out vec2 texCoord;

// Definition of struct PointLight
struct PointLight {
				   // Offset, Size
    vec3 position; // 0, 12
    
    float constant; // 12, 4
    float linear; // 16, 4
    float quadratic; // 20, 4
	
    vec3 ambient; // 32, 12
    vec3 diffuse; // 48, 12
    vec3 specular; // 64, 12
};

// Uniform block for model and view matrices, directional and point lights
layout (std140) uniform MatrixMV // 208
{
				// Offset, Size
	mat4 model; // 0, 64
	mat4 view; // 64, 64
	
	//PointLight vPointLights[NO_OF_POINTLIGHTS]; // 128, 80 * NO_OF_POINTLIGHTS
};

// Uniform block for projection matrix
layout (std140) uniform MatrixP
{
	mat4 projection;
};

void main()
{
	// Pass on texture coordinates
	texCoord = inTexCoord;
	
	// Convert vertex to view space
	gl_Position = projection * model * vec4(vertexPosition_modelspace, 1.0f);
}