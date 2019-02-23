#ifndef USEFULFUNCTIONS_H
#define USEFULFUNCTIONS_H
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Vector3.h"

const bool isPressed(GLFWwindow* window, GLenum key); // Checks if a key is pressed
bool linePlaneIntersect(const Vector3& linePoint, const Vector3& lineDir, const Vector3& planePoint, const Vector3& planeNorm, Vector3& output); // Writes result to output and returns true only if line intersects plane in positive direction
void updateUBO(GLenum UBO, GLint offset, GLsizeiptr size, const GLvoid* data);

#endif // !USEFULFUNCTIONS_H
