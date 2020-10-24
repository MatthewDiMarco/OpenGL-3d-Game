#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <list>

#include <stdlib.h>
#include <math.h>
#include <map>
#include <tuple>

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
	glm::vec3 ePos, eDir, eFront, eUp, eRight, ancor; // ancor to rotate on
	glm::vec3 *scales;
	glm::vec3 *positions;
	unsigned int *textures;
	int numTextures, numModels;
	float spd, yaw, pitch, roll;
	bool alive;
	std::map<int, std::tuple<float, float>> pitchAnimation; //<model idx, amount to increment>
	
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
		alive = true;
		ePos = inPos;
		eFront = inFront;
		eUp = inUp;
		eDir = calcDirection();
		eRight = calcRight();
		ancor = ePos;

		textures = NULL; //todo

		spd = SPD_DEFAULT;

		yaw = 0.0f;
		pitch = PITCH_DEFAULT;
		roll = 0.0f;
	}

	// Getters
	glm::vec3 getPosition() { return ePos; }
	glm::vec3 getDirection() { return eDir; }
	glm::vec3 getFront() { return eFront; }
	glm::vec3 getUp() { return eUp; }
	glm::vec3 getRight() { return eRight; }
	float getSpeed() { return spd; }
	glm::vec3 getScale(int idx) { return scales[idx]; }
	glm::vec3 getAncor() { return ancor; }
	float getYaw() { return yaw; }
	float getPitch() { return pitch; }
	float getRoll() { return roll; }
	bool isAlive() { return alive; }

	virtual void die() {
		alive = false;
	}

	void setPosition(glm::vec3 newPos)
	{
		ePos = newPos;
		eDir = calcDirection();
		eRight = calcRight();
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
		bool animated = false;
		float originalPitch = pitch;
		try {
			float const upperThreshold = 25.0f;
			float const lowerThreshold = -25.0f;
			float ptch = std::get<0>(pitchAnimation.at(ii));
			float increment = std::get<1>(pitchAnimation.at(ii));
			ptch += increment;
			setPitch(ptch);
			//std::cout << pitch << "\n";
			if (pitch > upperThreshold || pitch < lowerThreshold) {
				std::get<1>(pitchAnimation.at(ii)) = -increment;
			} 
			std::get<0>(pitchAnimation.at(ii)) = pitch;
			animated = true;
 
		} catch (const std::out_of_range& oor) {
			setPitch(originalPitch);		
		}
	
		currModel = glm::translate(currModel, ancor);

		if (yaw != 0) {
			currModel = glm::rotate(currModel, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		if (pitch != 0) {
			currModel = glm::rotate(currModel, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
			if (animated) setPitch(originalPitch);
		}
		if (roll != 0) {
			currModel = glm::rotate(currModel, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		//If the ancor IS NOT ePos, we must first convert it back
		if (ancor.x != ePos.x || ancor.y != ePos.y || ancor.z != ePos.z) { 
			currModel = glm::translate(currModel, -ancor);
			currModel = glm::translate(currModel, ePos);
		}

		currModel = glm::translate(currModel, positions[ii]);
		currModel = glm::scale(currModel, scales[ii]);

		return currModel;
	}

	virtual void render(unsigned int VAO_box, Shader shader)
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
			currModel = glm::mat4();
			currModel = doTransformations(currModel, ii);
			
			shader.setMat4("model", currModel);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

	void move(glm::vec3 offset)
	{
		ePos += offset;
		ancor += offset;
	}

	void setYaw(float a) { yaw = a; }
	void setPitch(float p) { pitch = p; }
	void setRoll(float r) { roll = r; }

	void changeYawBy(float yaw_offset)
	{
		yaw += yaw_offset;
		while (yaw > 360) yaw -= 360;
		while (yaw < 0) yaw += 360;
	}

	void changePitchBy(float pitch_offset)
	{
		pitch += pitch_offset;
		while (pitch > 89.0f) pitch -= 89.0f;
		while (pitch < -89.0f) pitch += 89.0f;
	}

	void setAncor(glm::vec3 a)
	{
		ancor = a;
	}

	void setSpeed(float newSpeed)
	{
		spd = newSpeed;
	}

	void setPitchAnimation(int idx, float inc)
	{
		pitchAnimation.insert(std::pair<int, std::tuple<float, float>>(
			idx, std::tuple<float, float>{pitch, inc})
		);
	}
};

// Description of the player camera
class Camera : public Entity
{

private:
	
	// Fields
	float lastX, lastY, sensitivity; // old x & y positions of mouse
	bool firstMouse, itemVisible;
	Entity* item;

public:	
	
	// Constructor
	Camera(glm::vec3 camPos, glm::vec3 camFront, glm::vec3 camUp, int scrWidth, int scrHeight) 
	: Entity(camPos, camFront, camUp)  
	{
		yaw = YAW_DEFAULT;	
		sensitivity = SENSITIVITY_DEFAULT;
		lastX = scrWidth / 2.0f; 
		lastY = scrHeight / 2.0f;
		firstMouse = true;
		itemVisible = false;
		item = NULL;
	}

	void setItem(Entity* inE)
	{
		// object should float in-front of the camera view
		glm::vec3 offset = glm::vec3(0.1f, -0.1, -0.3f);
		item = inE;
		item->setPosition(ePos);
		item->move(offset);
		item->setAncor(ePos);
	}

	void setItemVisible(bool b)
	{
		itemVisible = b;
	}
	
	void mouseMoved(double xpos, double ypos) 
	{
		if (alive)
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

			// Update item angle
			if (item != NULL)
			{
				item->changeYawBy(-xoffset);
				if ((item->getPitch()+yoffset) < 89.0f && (item->getPitch()+yoffset) > -89.0f) {
					item->changePitchBy(yoffset);
				}
			}

			// Constraints -- ensure we don't flip the direction vector
			if (pitch > 89.0f)
			{
				pitch = 89.0f;
			}
			else if (pitch < -89.0f)
			{
				pitch = -89.0f;
			}
			while (yaw > 360) yaw -= 360;
			while (yaw < 0) yaw += 360;

			// Update vectors
			glm::vec3 direction;
			direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			direction.y = sin(glm::radians(pitch));
			direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			eFront = glm::normalize(direction); 
			eRight = glm::normalize(glm::cross(eFront, eUp));
		}
	}

	void move(glm::vec3 offset)
	{
		Entity::move(offset);
		if (item != NULL) {
			item->move(offset);
		} 
	}

	void render(unsigned int VAO_box, Shader lighting_shader)
	{
		if (item != NULL && itemVisible)
		{
			item->render(VAO_box, lighting_shader);
		}
	}

	void die() 
	{
		Entity::die();
		ePos = glm::vec3(ePos.x, 0.1, ePos.z);
	}
};

class Pickup : public Entity
{

private:

	// Fields
	float translation;
	bool rotate;

public:

	// Constructor
	Pickup(glm::vec3 pPos, glm::vec3 pFront, glm::vec3 pUp) 
	: Entity(pPos, pFront, pUp)
	{
		translation = 0.0f;
		rotate = true;
	}
		
	void setRotateAnimation(bool b)
	{
		rotate = b;
	}
		
	glm::mat4 doTransformations(glm::mat4 currModel, int ii)
	{
		// Up down "bobbing" animation
		currModel = glm::translate(
			currModel, 
			glm::vec3(0.0f, (0.1f * sin(translation * PI / 180.f)), 0.0f)
		);

		currModel = Entity::doTransformations(currModel, ii);

		return currModel;
	}

	void render(unsigned int VAO_box, Shader lighting_shader)
	{
		translation += ANIMATION_SPEED;
		if(abs(translation - 360.0f) <= 0.1f) translation = 0.0f;

		if (rotate) {	
			changeYawBy(ANIMATION_SPEED);
		}

		Entity::render(VAO_box, lighting_shader);
	}
};

class Enemy : public Entity
{

private:

	// Fields
	Entity* target;

public:

	// Constructor
	Enemy(glm::vec3 pPos, glm::vec3 pFront, glm::vec3 pUp, Entity* inTarget) 
	: Entity(pPos, pFront, pUp)
	{
		target = inTarget;
	}

	void render(unsigned int VAO_box, Shader lighting_shader)
	{
		Entity::render(VAO_box, lighting_shader);
		//set angle
		
		// Move towards target
		float threshold = 1.0f;
		if (glm::length(ePos - target->getPosition()) > threshold && target->isAlive())
		{
			glm::vec3 dir = target->getPosition() - ePos;
			
			setYaw(glm::degrees(atan2(dir.x, dir.z)));

			eFront = glm::normalize(dir); 
			eRight = glm::normalize(glm::cross(eFront, eUp));

			move(eFront * spd);
		}
		else
		{
			target->die();
		}
	}
};
