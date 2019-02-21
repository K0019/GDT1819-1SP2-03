#version 330 core

/* File: TextFragmentShader.frag
   Use: To sample a texture
		Only applicable to 2D sprites */

// Definition of struct Material
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

in vec2 texCoord; // Texture coordinate of vertex

// Ouput data
out vec4 color;

// Texture to sample from
uniform Material material;

uniform float alpha;

void main()
{
	// Apply color sampled from texture
	color = texture(material.diffuse, texCoord);
	color.w *= alpha;
}