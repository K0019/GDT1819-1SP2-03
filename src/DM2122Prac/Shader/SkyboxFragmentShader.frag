#version 330 core

/* File: SkyboxFragmentShader.frag
   Use: Handle texturing of skybox mesh
		Shader applicable to the skybox only */

// Ouput data
out vec4 FragColor;

// Direction to sample cubemap
in vec3 texCoord;

// Cubemap texture sampler
uniform samplerCube skybox;

void main()
{
	// Get texture color
	FragColor = texture(skybox, texCoord);
}