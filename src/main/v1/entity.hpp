#include <glm/glm.hpp>
#include <GLFW/glfw3.h> 

// Constants
static const glm::vec3 	ORIGIN 				= glm::vec3(0.0f, 0.0f, 0.0f);
static const float 		YAW_DEFAULT 		= -90.0f;
static const float 		PITCH_DEFAULT 		= 0.0f;
static const float 		SENSITIVITY_DEFAULT = 0.05f;

// Abstract description of a "thing" in the world
class Entity 
{

protected:

	// Fields
	glm::vec3 ePos, eDir, eFront, eUp, eRight;
	
	// Helpers
	glm::vec3 calcDirection() 
	{
		return glm::normalize(ePos - ORIGIN);
	}
	glm::vec3 calcRight()
	{
		return glm::normalize(glm::cross(eUp, eDir));
	}

public:

	// Constructor with vectors
	Entity(glm::vec3 inPos, glm::vec3 inFront, glm::vec3 inUp) 
	{
		ePos = inPos;
		eFront = inFront;
		eUp = inUp;
		eDir = calcDirection();
		eRight = calcRight();
	}

	// Getters
	glm::vec3 getPosition() { return ePos; }
	glm::vec3 getDirection() { return eDir; }
	glm::vec3 getFront() { return eFront; }
	glm::vec3 getUp() { return eUp; }
	glm::vec3 getRight() { return eRight; }

	// Setters	
	void setPosition(glm::vec3 inPos)
	{
		ePos = inPos;
		eDir = calcDirection(); 
	}
	void setFront(glm::vec3 inFront)
	{
		eFront = inFront;
	}
	void setUp(glm::vec3 inUp)
	{
		eUp = inUp;
		eRight = calcRight();
	}	
};

// Description of the player camera
class Camera : public Entity
{
private:
	
	// Fields
	float yaw, pitch; // left-to-right and up-down roation, respectively
	float lastX, lastY, sensitivity; // old x & y positions of mouse
	bool firstMouse;

public:	
	
	// Constructor
	Camera(glm::vec3 camPos, glm::vec3 camFront, glm::vec3 camUp, int scrWidth, int scrHeight) 
	: Entity(camPos, camFront, camUp)  
	{	
		yaw = YAW_DEFAULT;
		pitch = PITCH_DEFAULT;
		sensitivity = SENSITIVITY_DEFAULT;
		lastX = scrWidth / 2.0f; 
		lastY = scrHeight / 2.0f;
		firstMouse = true;
	}
	
	void mouseMoved(double xpos, double ypos) 
	{
		if (firstMouse) 
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		// Update coords
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		// Apply sensitivity
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		// Modify cam angle
		yaw += xoffset;
		pitch += yoffset;

		// Constraints -- ensure we don't flip the direction vector
		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}
		else if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}

		// Update vectors
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		eFront = glm::normalize(direction); 
		eRight = glm::normalize(glm::cross(eFront, eUp));
	}
};
