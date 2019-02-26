#version 330 core

/* File: TextureColorVertexShader.vert
   Use: Handle conversion of mesh in model space to view space 
		General shader used in most meshes */

// No. of point lights and spotlights
#define NO_OF_POINTLIGHTS 0
#define NO_OF_SPOTLIGHTS 4

// Info about primitive
layout(location = 2) in vec3 vertexPosition_modelspace;
layout(location = 5) in vec2 inTexCoord;

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

// Definition of struct SpotLight
struct SpotLight {
				   // Offset, Size
	vec3 position; // 0, 12
	
	float constant; // 12, 4
	float linear; // 16, 4
	float quadratic; // 20, 4
	
	vec3 ambient; // 32, 12
	vec3 diffuse; // 48, 12
	vec3 specular; // 64, 12
	
	vec3 direction; // 80, 12
	float cosInner; // 92, 4
	float cosOuter; // 96, 4
};

// Output to geometry shader
out VS_OUT
{
	vec3 aPos; // Vertex info
	vec2 aTexCoord; // Texture info

	//vec3 aSunDir; // Directional light
	//PointLight aPointLights[NO_OF_POINTLIGHTS]; // Point lights
	//SpotLight aSpotLights[NO_OF_SPOTLIGHTS]; // Spotlights
} vs_out;

// Uniform block for model and view matrices, directional and point lights
layout (std140) uniform MatrixMV // 208
{
				// Offset, Size
	mat4 model; // 0, 64
	mat4 view; // 64, 64
	
	vec3 sunDir; // 128, 12
	//PointLight vPointLights[NO_OF_POINTLIGHTS]; // 144, 80 * NO_OF_POINTLIGHTS
};

// Uniform block for spotlights
/*layout (std140) uniform uSpotLight
{
	SpotLight vSpotLights[NO_OF_SPOTLIGHTS]; // 0, 112 * NO_OF_SPOTLIGHTS
};*/

void main()
{
	// Convert vertex to view space
    gl_Position = view * model * vec4(vertexPosition_modelspace,1);

	// Interpolate fragment coordinate
	vs_out.aPos = vec3(view * model * vec4(vertexPosition_modelspace, 1.0f));
	vs_out.aTexCoord = inTexCoord;
	
	// Convert directional light direction to view space
	//vs_out.aSunDir = normalize(vec3(mat4(mat3(view)) * vec4(sunDir, 1.0f)));

	// Convert point lights' positions to view space
	/*for (int i = 0; i < NO_OF_POINTLIGHTS; ++i)
	{
		vs_out.aPointLights[i] = vPointLights[i];
		vs_out.aPointLights[i].position = vec3(view * vec4(vPointLights[i].position, 1.0f));
	}*/
	
	// Convert spotlights' positions to view space
	/*for (int i = 0; i < NO_OF_SPOTLIGHTS; ++i)
	{
		vs_out.aSpotLights[i] = vSpotLights[i];
		vs_out.aSpotLights[i].position = vec3(view * vec4(vSpotLights[i].position, 1.0f));
		vs_out.aSpotLights[i].direction = vec3(mat4(mat3(view)) * vec4(vSpotLights[i].direction, 1.0f));
	}*/
}

