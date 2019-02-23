#version 330 core

/* File: LampVertexShader.vert
   Use: Handle conversion of mesh in model space to view space 
		Only used to handle rendering of lamp (sphere). All other meshes except axes use SingleColorXXXXShader.XXXX */

// No. of point lights
#define NO_OF_POINTLIGHTS 0

// Info about primitive
layout(location = 2) in vec3 vertexPosition_modelspace; // Vertex info

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

// Output to geometry shader
out VS_OUT
{
	vec3 aPos; // Vertex info
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

void main()
{
	// Convert vertex to view space
    gl_Position = view * model * vec4(vertexPosition_modelspace, 1.0f);

	// Interpolate fragment coordinate
	vs_out.aPos = vec3(view * model * vec4(vertexPosition_modelspace, 1.0f));
}

