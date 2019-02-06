#version 330 core

/* File: AxesFragmentShader.frag
   Use: To simply apply color supplied to primitive
		Only used to handle rendering of axes. All other meshes except lamp use SingleColorXXXXShader.XXXX */

// Input from geometry shader
in vec3 fColor; // Color info
in vec3 pos; // Vertex info

// Ouput data
out vec4 color; // Color info

void main()
{
	// Simply pass on the color to OpenGL
	color = vec4(fColor, 1.0f);
}