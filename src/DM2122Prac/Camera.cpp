#include "Camera.h"

// Initialize static variables
bool Camera::firstMouse = true;
float Camera::mouseX = (float)(800 << 1);
float Camera::mouseY = (float)(600 << 1);

// Constructor
Camera::Camera()
{
	init(); // Initialize camera to default values
}

// Sets default values for camera
void Camera::init()
{
	pos = { 0.0f, 5.0f, -5.0f };
	front = { 0.0f, 0.0f, 1.0f };
	up = { 0.0f, 1.0f, 0.0f };
	yaw = 90.0f;
	pitch = 0.0f;
}

// Set camera position
void Camera::setPos(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

// Set camera position
void Camera::setPos(const Vector3& position)
{
	pos = position;
}

// Set camera front
void Camera::setFront(float x, float y, float z)
{
	front.x = x;
	front.y = y;
	front.z = z;
}