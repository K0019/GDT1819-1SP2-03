#version 330 core

/* File: LampFragmentShader.frag
   Use: To simply apply the color white to primitive
		Only used to handle rendering of lamps. All other meshes except lamp use SingleColorXXXXShader.XXXX */

// Ouput data
out vec4 color;

void main()
{
	// Set the color of primitive to white
	color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}