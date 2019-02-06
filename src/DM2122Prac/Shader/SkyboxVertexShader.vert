#version 330 core

/* File: SkyboxVertexShader.vert
   Use: Handle conversion of mesh in model space to camera space 
		Shader applicable to the skybox only */
		
// No. of point lights
#define NO_OF_POINTLIGHTS 1

// Info about primitive
layout (location = 2) in vec3 vertexPosition_modelspace;

// Direction to sample cubemap
out vec3 texCoord;

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
	
	vec3 sunDir; // 128, 12
	PointLight vPointLights[NO_OF_POINTLIGHTS]; // 144, 80 * NO_OF_POINTLIGHTS
};

// Uniform block for projection matrix
layout (std140) uniform MatrixP
{
	mat4 projection;
};

void main()
{
	// Remove translation aspect of view matrix
	mat4 newView = mat4(mat3(view));

	// Pass on direction to sample texture
	texCoord = vertexPosition_modelspace;
	
	// Apply transformation
	gl_Position = projection * newView * vec4(vertexPosition_modelspace, 1.0f);
}