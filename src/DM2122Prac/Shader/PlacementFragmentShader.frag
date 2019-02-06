#version 330 core

/* File: PlacementFragmentShader.frag
   Use: To apply a translucent color to the primitive 
		Only used to handle rendering of a non-placed meshPlaceable */

// Ouput data
out vec4 color;

// Color to render the mesh
uniform vec3 glowColor;

void main()
{
	// Apply the color
	color = vec4(glowColor, 0.5f);
}