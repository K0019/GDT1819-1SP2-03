#version 330 core

/* File: SingleColorFragmentShader.frag
   Use: To apply lighting to the primitive 
		General shader used in most meshes */
		
// No. of point lights
#define NO_OF_POINTLIGHTS 1

// Definition of struct PointLight
struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Definition of struct Material
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

// Input from geometry shader
in vec3 normal; // Normal of primitive
in vec3 pos; // Position of vertex in view space
in vec2 texCoord; // Texture coordinate of vertex
in vec3 sunDir;
in PointLight cPointLights[NO_OF_POINTLIGHTS]; // All the point lights

// Ouput data
out vec4 color;

uniform Material material;

void main()
{
	color = texture(material.diffuse, texCoord);
}