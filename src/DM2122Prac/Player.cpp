#include "Player.h"

Camera Player::camera = Camera();

// Function called when mouse moves to update camera
void Player::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	// Avoid camera movement for first frame, to avoid large camera jump
	if (camera.firstMouse)
	{
		camera.mouseX = (float)xpos;
		camera.mouseY = (float)ypos;
		camera.firstMouse = false;
	}
	// Update internal cursor position
	float xOffset = (float)(xpos - camera.mouseX);
	float yOffset = (float)(camera.mouseY - ypos);
	camera.mouseX = (float)xpos;
	camera.mouseY = (float)ypos;

	// Multiply by sensitivity
	float sensitivity = 0.05f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	// Modify camera pitch and yaw, clamping it to avoid Gimbal Lock
	camera.yaw += xOffset;
	camera.pitch += yOffset;
	if (camera.pitch > 89.9f)
		camera.pitch = 89.9f;
	else if (camera.pitch < -89.9f)
		camera.pitch = -89.9f;

	// Update camera front
	camera.front = Vector3((float)(cos(Math::DegreeToRadian(camera.pitch)) * cos(Math::DegreeToRadian(camera.yaw))),
		(float)(sin(Math::DegreeToRadian(camera.pitch))),
		(float)(cos(Math::DegreeToRadian(camera.pitch)) * sin(Math::DegreeToRadian(camera.yaw)))).Normalize();
}

// Constructor
Player::Player(unsigned int uMatrixMVS, unsigned int uColorData)
	: jumping(false)
	, sprinting(false)
	, kart(nullptr)
	, velocity(Vector3())
	, bear(new Bear(uMatrixMVS, uColorData))
{

}

// Destructor
Player::~Player()
{
	delete bear;
}

// Handle input
void Player::update(GLFWwindow* window, double deltaTime, unsigned int uSpotLight)
{
	// TODO: Collision

	// If kart is not attached
	if (kart == nullptr)
	{
		float cameraSpeed = (float)(6.0f * deltaTime); // Camera speed
	
		// Horizontal movement
		if (!jumping)
		{
			// Modify camera according to respective direction if key is pressed

			if (isPressed(window, GLFW_KEY_W))
				//camera.pos += cameraSpeed * Vector3(camera.front.x, 0.0f, camera.front.z).Normalize();
				velocity += Vector3(camera.front.x, 0.0f, camera.front.z).Normalize();
			if (isPressed(window, GLFW_KEY_S))
				//camera.pos -= cameraSpeed * Vector3(camera.front.x, 0.0f, camera.front.z).Normalize();
				velocity -= Vector3(camera.front.x, 0.0f, camera.front.z).Normalize();
			if (isPressed(window, GLFW_KEY_A))
				//camera.pos -= camera.front.Cross(camera.up).Normalize() * cameraSpeed;
				velocity -= camera.front.Cross(camera.up).Normalize();
			if (isPressed(window, GLFW_KEY_D))
				//camera.pos += camera.front.Cross(camera.up).Normalize() * cameraSpeed;
				velocity += camera.front.Cross(camera.up).Normalize();

			// Sprinting
			sprinting = isPressed(window, GLFW_KEY_LEFT_SHIFT);

			// Normalize velocity
			if (velocity != Vector3())
			{
				velocity.Normalize();
			}
		}

		// Jumping
		if (!jumping && isPressed(window, GLFW_KEY_SPACE))
		{
			jumping = true;

			velocity.y = 3.5f;
			//camera.pos += cameraSpeed * camera.up;
		}

		// Move player vertically first
		camera.pos.y += velocity.y * cameraSpeed;

		// Then increase player speed if sprinting
		if (sprinting)
			cameraSpeed *= 1.7f;
		// Move player horizontally
		camera.pos.x += velocity.x * cameraSpeed;
		camera.pos.z += velocity.z * cameraSpeed;

		// Gravity
		velocity -= Vector3(0.0f, 10.0f, 0.0f) * static_cast<float>(deltaTime);

		// Collision with ground
		if (camera.pos.y < 3.5f)
		{
			jumping = false;

			camera.pos.y = 3.5f;
			
			velocity = Vector3();
		}

	}
	else // If kart is attached
	{
		// Update the kart
		kart->update(window, deltaTime, uSpotLight);
		
		// Update camera
		camera.pos = kart->getPos();
		double effectiveYaw = kart->getYaw();
		// Camera position rotation
		if (isPressed(window, GLFW_KEY_DOWN))
			effectiveYaw += 180.0;
		else if (isPressed(window, GLFW_KEY_LEFT))
			effectiveYaw += 90.0;
		else if (isPressed(window, GLFW_KEY_RIGHT))
			effectiveYaw += 270.0;

		camera.pos.x -= sinf(Math::DegreeToRadian(static_cast<float>(effectiveYaw))) * 10.0f - kart->getCamDir().x;
		camera.pos.y += 6.f - kart ->getCamDir().y;
		camera.pos.z -= cosf(Math::DegreeToRadian(static_cast<float>(effectiveYaw))) * 10.0f - kart->getCamDir().z;
		/*camera.pos.x = kart->getPos().x;
		camera.pos.y = 10.0f;
		camera.pos.z = kart->getPos().z + 0.1f;**/

		camera.front = (Vector3(kart->getPos().x, kart->getPos().y + 5.0f, kart->getPos().z) - camera.pos).Normalize();
	}

	/*if (isPressed(window, GLFW_KEY_LEFT_SHIFT))
		camera.pos -= cameraSpeed * camera.up;*/
}

// Render bear if player is driving the kart
void Player::render() const
{
	if (hasKart())
	{
		bear->draw(kart->getPos(), static_cast<float>(kart->getYaw() + 180.0));
	}
}

// Attach kart object to player
void Player::attachKart(Kart* kart)
{
	if (kart == nullptr && this->kart != nullptr)
	{
		// If kart has any velocity, stop it first
		this->kart->stop();

		// Player will teleport to within mid-air, so position updating should perform like the player is in mid-air when player exits
		jumping = true;

		// Update camera
		camera.yaw = static_cast<float>(this->kart->getYaw()) + 90.0f;
		camera.pitch = 0.0f;

		camera.front = Vector3((float)(cos(Math::DegreeToRadian(camera.pitch)) * cos(Math::DegreeToRadian(camera.yaw))),
			(float)(sin(Math::DegreeToRadian(camera.pitch))),
			(float)(cos(Math::DegreeToRadian(camera.pitch)) * sin(Math::DegreeToRadian(camera.yaw)))).Normalize();
	}
	// Attach kart
	this->kart = kart;
}

// Check if a kart is attached to the player
bool Player::hasKart() const
{
	return kart != nullptr;
}
// Reset player position and orientation
void Player::reset()
{
	camera.init();
}
// Return reference to camera object
const Camera& Player::getCam() const
{
	return camera;
}