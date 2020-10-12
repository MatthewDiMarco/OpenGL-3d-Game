#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <list>

#include <glm/gtc/matrix_transform.hpp>

#include <learnopengl/shader_m.h> 

#define PI 3.14159265

// Constants
static const float YAW_DEFAULT 			= -90.0f;
static const float PITCH_DEFAULT 		= 0.0f;
static const float SPD_DEFAULT			= 2.5f;
static const float SENSITIVITY_DEFAULT 	= 0.05f;
static const float ANIMATION_SPEED 		= 6.0f;

// Abstract description of a "thing" in the world
class Entity 
{

protected:

	// Fields
	glm::vec3 ePos, eDir, eFront, eUp, eRight;
	glm::vec3 *scales;
	glm::vec3 *positions;
	unsigned int *textures;
	int numTextures, numModels;
	float spd, angle;
	
	// Helpers
	glm::vec3 calcDirection() 
	{
		return glm::normalize(ePos - glm::vec3(0.0f, 0.0f, 0.0f));
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

		textures = NULL; //todo

		spd = SPD_DEFAULT;
		angle = 0;
	}

	// Getters
	glm::vec3 getPosition() { return ePos; }
	glm::vec3 getDirection() { return eDir; }
	glm::vec3 getFront() { return eFront; }
	glm::vec3 getUp() { return eUp; }
	glm::vec3 getRight() { return eRight; }
	float getSpeed() { return spd; }
	glm::vec3 getScale(int idx) { return scales[idx]; }	

	void setFront(glm::vec3 inFront)
	{
		eFront = inFront;
	}

	void setUp(glm::vec3 inUp)
	{
		eUp = inUp;
		eRight = calcRight();
	}
	
	void setModel(glm::vec3 inScales[], glm::vec3 inPositions[], int arrSize)
	{
		positions = inPositions;
		scales = inScales;
		numModels = arrSize;
	}

	void setTextures(unsigned int *inTextures, int numT)
	{
		textures = inTextures;
		numTextures = numT;
	}

	virtual glm::mat4 doTransformations(glm::mat4 currModel, int ii)
	{	
		currModel = glm::mat4();
		currModel = glm::translate(currModel, ePos);
		if (angle != 0) {
			currModel = glm::rotate(currModel, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		currModel = glm::translate(currModel, positions[ii]);
		currModel = glm::scale(currModel, scales[ii]);

		return currModel;
	}

	virtual void render(unsigned int VAO_box, Shader lighting_shader)
	{
		glBindVertexArray(VAO_box);

		unsigned int glTex[] = {GL_TEXTURE0, GL_TEXTURE1};
		
		for (int ii = 0; ii < numTextures; ii++)
		{
			glActiveTexture(glTex[ii]);
			glBindTexture(GL_TEXTURE_2D, textures[ii]);
		}

		// Construct the model(s)
		glm::mat4 currModel;
		for (int ii = 0; ii < numModels; ii++)
		{
			currModel = doTransformations(currModel, ii);
			
			lighting_shader.setMat4("model", currModel);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

	void move(glm::vec3 offset)
	{
		ePos += offset;
	}

	void changeAngleBy(float angle_offset)
	{
		angle += angle_offset;	
		while (angle > 360) angle -= 360;
		while (angle < 0) angle += 360;
		std::cout << angle << "\n";
	}

	void setSpeed(float newSpeed)
	{
		spd = newSpeed;
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

class Pickup : public Entity
{

private:

	// Fields
	float translation, rotation;

public:

	// Constructor
	Pickup(glm::vec3 pPos, glm::vec3 pFront, glm::vec3 pUp) 
	: Entity(pPos, pFront, pUp)
	{
		translation = 0.0f;
		rotation = 0.0f;
	}
			
	glm::mat4 doTransformations(glm::mat4 currModel, int ii)
	{
		// Up down "bobbing" animation
		currModel = glm::mat4();
		currModel = glm::translate(
			currModel, 
			glm::vec3(0.0f, (0.1f * sin(translation * PI / 180.f)), 0.0f)
		);

		// Move to respective position
		currModel = glm::translate(currModel, ePos);

		// Rotation
		currModel = glm::rotate(currModel, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		currModel = glm::translate(currModel, positions[ii]);
	
		// Bring the model to scale
		currModel = glm::scale(currModel, scales[ii]);

		return currModel;
	}

	void render(unsigned int VAO_box, Shader lighting_shader)
	{
		translation += ANIMATION_SPEED;
		rotation += ANIMATION_SPEED;
		if(abs(translation - 360.0f) <= 0.1f) translation = 0.0f;
		if(abs(rotation - 360.0f) <= 0.1f) rotation = 0.0f;
	
		Entity::render(VAO_box, lighting_shader);
	}
};
