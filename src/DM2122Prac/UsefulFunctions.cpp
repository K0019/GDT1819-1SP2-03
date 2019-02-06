#include "UsefulFunctions.h"

// Checks if a key is pressed
const bool isPressed(GLFWwindow* window, GLenum key)
{
	return glfwGetKey(window, key) == GLFW_PRESS;
}

// Writes result to output and returns true only if line intersects plane in positive direction
bool linePlaneIntersect(const Vector3& linePoint, const Vector3& lineDir, const Vector3& planePoint, const Vector3& planeNorm, Vector3& output)
{
	double lambdaCoefficient = lineDir.Dot(planeNorm);
	if (lambdaCoefficient == 0.0)
		return false;
	double rhs = planePoint.Dot(planeNorm) - linePoint.Dot(planeNorm);
	double lambda = rhs / lambdaCoefficient;
	if (lambda <= 0.0)
		return false;

	output = Vector3(static_cast<float>(linePoint.x + lambda * lineDir.x), 
					 static_cast<float>(linePoint.y + lambda * lineDir.y),
					 static_cast<float>(linePoint.z + lambda * lineDir.z));
	return true;
}