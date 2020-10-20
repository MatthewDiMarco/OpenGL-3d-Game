#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>

#include <iostream>
#include <string>

#include "entity.hpp"

// Constants
const char* GAME_TITLE = "Escape Game";

// Screen
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
glm::mat4 perspective;
glm::mat4 orthographic;

// World
static const float WORLD_WIDTH = 70.0f;
static const float WORLD_LENGTH = 70.0f;
static const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
static const glm::vec3 NORTH = glm::vec3(0.0f, 0.0f, -1.0f);
static const glm::vec3 ORIGIN = glm::vec3(0.0f, 0.0f, 0.0f);
static const float INTERACT_DISTANCE = 1.6f;
static float lightSourceRadius = 0.5f;
Shader* light;
std::list<Entity*> entities;
std::list<Pickup*> pickups;

// Textures
unsigned int* wood_textures;
unsigned int* brick_textures;
unsigned int* grass_textures;
unsigned int* road_textures;
unsigned int* marble_textures;

// Box coordinate with 36 vertices.
// Every 3 coordinates will form 1 triangle.
// The last 2 columns represent texture coordinate for mapping.
float box[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

// Model Descriptions -- positions and scales
// The positions are relative to the entity's position vector, not origin.
// i.e. "origin" for the model is the position of the entity it is attached to, which could be
// any coordinate in the world.

// Wall barriers
glm::vec3 wall_scales[] = {
	glm::vec3( WORLD_WIDTH,  10.0f,  0.01f),	//North
	glm::vec3( WORLD_WIDTH,  10.0f,  0.01f),	//South
	glm::vec3( 0.01f,  10.0f,  WORLD_LENGTH),	//East
	glm::vec3( 0.01f,  10.0f,  WORLD_LENGTH),	//West
};
glm::vec3 wall_positions[] = {
	glm::vec3( 0.0f,  			0.0f,  -(WORLD_LENGTH/2)),	//North
	glm::vec3( 0.0f,  			0.0f,   (WORLD_LENGTH/2)),	//South
	glm::vec3( (WORLD_WIDTH/2), 0.0f,   0.0f),				//East
	glm::vec3(-(WORLD_WIDTH/2), 0.0f,   0.0f),				//West
};

// Table
glm::vec3 table_scales[] = {
	glm::vec3( 1.0f,  0.1f,  1.0f),		//top
	glm::vec3( 0.1f,  0.5f,  0.1f),		//near left
	glm::vec3( 0.1f,  0.5f,  0.1f),		//near right
	glm::vec3( 0.1f,  0.5f,  0.1f),		//far left
	glm::vec3( 0.1f,  0.5f,  0.1f),		//far right
};
glm::vec3 table_positions[] = {
	glm::vec3( 0.0f,  0.5f,  0.0f),		//top
	glm::vec3(-0.45f, 0.2f,  0.45f),	//near left
	glm::vec3( 0.45f, 0.2f,  0.45f),	//near right
	glm::vec3(-0.45f, 0.2f, -0.45f),	//far left
	glm::vec3( 0.45f, 0.2f, -0.45f),	//far right
};

// Box
glm::vec3 pickup_scales[] = {
	glm::vec3( 0.2f,  0.2f,  0.2f)
};
glm::vec3 pickup_positions[] = {
	glm::vec3( 0.0f,  0.0f,  0.0f)
};

// Torch
glm::vec3 torch_scales[] = {
	glm::vec3( 0.03f,  0.03f,  0.02f),
	glm::vec3( 0.02f,  0.02f,  0.1f)
};
glm::vec3 torch_positions[] = {
	glm::vec3( 0.0f,  0.0f,  0.0f), // Torch head
	glm::vec3( -0.002f,  0.002f,  0.05f) 	// Handle
};

// Entities
Entity *walls, *player, *table, *tEquip; //Equiped version of the torch
Camera *cam;
Pickup *torch;
Entity* light_source; // define what entity is "producing" the light

// Prototype Declarations
void mouse_callback(GLFWwindow* window, double xpos, double ypos); 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow *window);
unsigned int loadTexture(char const * path);
bool collisionAt(glm::vec3 position);
void addEntity(
	Entity* e, 
	glm::vec3 scales[], glm::vec3 positions[], int numModel,
	unsigned int textures[], int numTextures);
void addPickup(
	Pickup* p, 
	glm::vec3 scales[], glm::vec3 positions[], int numModel,
	unsigned int textures[], int numTextures);
bool is_close_to(glm::vec3 entity_pos);
void start();
 
// timing
float delta_time = 0.0f;	// time between current frame and last frame
float last_frame = 0.0f;

// Toggle (animation or states)
bool PERSPECTIVE_PROJECTION = true;
bool PROJECTION_UPDATED = false;
bool SCENERY_DARK = true;
bool INTERACTIVITY_CLOSE_ENOUGH = false;
int closest_pickup_idx = 0;

// Main Algorithm
// --------------
int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, GAME_TITLE, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Callback functions 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	// Capture and fixate cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Configure global opengl state
	glEnable(GL_DEPTH_TEST);

	// Build and compile our shader zprogram
	Shader lighting_shader("./sample2.vs", "./sample2.fs");
	light = &lighting_shader;
	
	// Set up vertex data (and buffer(s)) and configure vertex attributes
	unsigned int VBO_box, VAO_box;

	glGenVertexArrays(1, &VAO_box);
	glGenBuffers(1, &VBO_box);

	glBindVertexArray(VAO_box);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_box);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);

	//vertex coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//normal vectors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int VAO_light;
	glGenVertexArrays(1, &VAO_light);
	glBindVertexArray(VAO_light);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_box);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Textures
	wood_textures = new unsigned int[2];
	brick_textures = new unsigned int[2];
	grass_textures = new unsigned int[2];
	road_textures = new unsigned int[2];
	marble_textures = new unsigned int[2];

	wood_textures[0] = loadTexture(FileSystem::getPath(
		"resources/textures/wood2.jpg").c_str());
	wood_textures[1] = loadTexture(FileSystem::getPath(
		"resources/textures/wood2_specular.jpg").c_str());

	brick_textures[0] = loadTexture(FileSystem::getPath(
		"resources/textures/brickwall.jpg").c_str());
	brick_textures[1] = loadTexture(FileSystem::getPath(
		"resources/textures/red_specular.jpg").c_str());

	grass_textures[0] = loadTexture(FileSystem::getPath(
		"resources/textures/grass.jpg").c_str());
	grass_textures[1] = loadTexture(FileSystem::getPath(
		"resources/textures/grass_specular.jpg").c_str());

	road_textures[0] = loadTexture(FileSystem::getPath(
		"resources/textures/street.png").c_str());
	road_textures[1] = loadTexture(FileSystem::getPath(
		"resources/textures/street_specular.png").c_str());

	marble_textures[0] = loadTexture(FileSystem::getPath(
		"resources/textures/marble2.jpg").c_str());
	marble_textures[1] = loadTexture(FileSystem::getPath(
		"resources/textures/marble_specular.jpg").c_str());

	// Initialise WORLD and ENTITIES
	// ------------------------------------------------------------------------------------------

	start();
	
	// Shader configuration 
	lighting_shader.use();
	lighting_shader.setInt("material.diffuse", 0);
	lighting_shader.setInt("material.specular", 1);

	// Define projection matricies and pass to shader
	// Can toggle between the two
	perspective = glm::perspective(
		glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);
	orthographic = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 5.0f, 100.0f);
	light->setMat4("projection", perspective); // TODO wasn't working cause this is outside loop!

	// Render Loop
	while (!glfwWindowShouldClose(window))
	{	
		// Per-frame time logic
		float currentFrame = glfwGetTime();
		delta_time = currentFrame - last_frame;
		last_frame = currentFrame;

		// Input
		process_input(window);

		// Render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		// Activate shader
		lighting_shader.use();
		light = &lighting_shader;
		lighting_shader.setVec3("light.position", light_source->getPosition());
		lighting_shader.setVec3("light.direction", light_source->getFront());
		lighting_shader.setFloat("light.cutOff", glm::cos(glm::radians(20.5f)));
		lighting_shader.setFloat("light.outerCutOff", glm::cos(glm::radians(22.5f)));
        lighting_shader.setVec3("viewPos", light_source->getPosition());

		// Light properties
		if (SCENERY_DARK) lighting_shader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
		else lighting_shader.setVec3("light.ambient", 1.0f, 1.0f, 1.0f);
		lighting_shader.setVec3("light.diffuse", 2.0f, 2.0f, 2.0f);
		lighting_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		lighting_shader.setFloat("light.constant", 1.0f);
		lighting_shader.setFloat("light.linear", lightSourceRadius);
		lighting_shader.setFloat("light.quadratic", 0.0032f);

		// Material properties
        lighting_shader.setFloat("material.shininess", 52.0f);

		// Camera/view transformation
		glm::mat4 view = glm::lookAt(
			cam->getPosition(), cam->getPosition() + cam->getFront(), cam->getUp()
		);
		light->setMat4("view", view);

		// Declare transformation matrix
		glm::mat4 model = glm::mat4();

		// Handle perspective switch
		if (PROJECTION_UPDATED)
		{
			if (PERSPECTIVE_PROJECTION)
			{
				light->setMat4("projection", orthographic);
			}
			else
			{
				light->setMat4("projection", perspective);
			}
		}

		// Draw objects
		// --------------------------------------------------------------------------------------
	
		//Street
		glBindVertexArray(VAO_box); 

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, road_textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, road_textures[1]);

		model = glm::mat4();
		model = glm::scale(model, glm::vec3(10.0f, 0.001f, WORLD_LENGTH));

		light->setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Grass
		glBindVertexArray(VAO_box);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, grass_textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, grass_textures[1]);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -0.01f, 0.0f));
		model = glm::scale(model, glm::vec3(WORLD_WIDTH, 0.001f, WORLD_LENGTH));

		light->setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		// Render the entities
		std::list<Entity*>::iterator it1 = entities.begin();
		for (int ii = 0; ii < (int)(entities.size()); ii++)
		{
			(*it1)->render(VAO_box, *light);
			std::advance(it1, 1);
		}

		// Render the pickups
		INTERACTIVITY_CLOSE_ENOUGH = false;
		std::list<Pickup*>::iterator it2 = pickups.begin();
		for (int ii = 0; ii < (int)(pickups.size()); ii++)
		{	
			(*it2)->render(VAO_box, *light);
			if (is_close_to((*it2)->getPosition()))
			{
				INTERACTIVITY_CLOSE_ENOUGH = true;
				closest_pickup_idx = ii; 
			}
			std::advance(it2, 1);
		}
	
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// De-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO_box);
	glDeleteBuffers(1, &VBO_box);

	delete wood_textures;
	delete brick_textures;
	delete grass_textures;
	delete road_textures;
	delete marble_textures;

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and 
// react accordingly.
// ---------------------------------------------------------------------------------------
void process_input(GLFWwindow *window)
{
	// Exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, true);
	}

	// Double speed when "Shift" pressed
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		cam->setSpeed(2.5 * delta_time); 
	else
		cam->setSpeed(2.5 * delta_time * 2);
	
	// Move around
	float cameraSpeed = cam->getSpeed();
	glm::vec3 newPos;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && 
	    !collisionAt(cam->getPosition() + cameraSpeed * player->getFront())) {
		cam->move(cameraSpeed * player->getFront());
		player->move(cameraSpeed * player->getFront());
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS &&
		!collisionAt(cam->getPosition() - cameraSpeed * player->getFront())) {
		cam->move(-cameraSpeed * player->getFront());
		player->move(-cameraSpeed * player->getFront());
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && 
		!collisionAt(cam->getPosition() - cam->getRight() * cameraSpeed)) {
		cam->move(-cam->getRight() * cameraSpeed);
		player->move(-cam->getRight() * cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && 
		!collisionAt(cam->getPosition() + cam->getRight() * cameraSpeed)) {
		cam->move(cam->getRight() * cameraSpeed);
		player->move(cam->getRight() * cameraSpeed);
	}

	// Restart
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		start();
	}
	
	// Pick stuff up
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && INTERACTIVITY_CLOSE_ENOUGH)
	{
		std::list<Pickup*>::iterator it = pickups.begin();
		std::advance(it, closest_pickup_idx);
		pickups.erase(it);
	
		if ((*it) == (torch)) {
			cam->setItemVisible(true);
			light_source = cam;
		}
	}

	// Perspective shift
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) 
	{
		PERSPECTIVE_PROJECTION = !PERSPECTIVE_PROJECTION;
		PROJECTION_UPDATED = true;
	}
	
	// Scenery shift
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		SCENERY_DARK = !SCENERY_DARK;	
	}

	// Increase brightness radius
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		lightSourceRadius += 0.1f;
		if (lightSourceRadius > 2.0f) {	
			lightSourceRadius = 2.0f;
		}
	}

	// Decrease brightness radius
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		lightSourceRadius -= 0.1f;
		if (lightSourceRadius < 0.0f) {	
			lightSourceRadius = 0.1f;
		}
	}
}

// What to do when the mouse moves 
// ---------------------------------------------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	cam->mouseMoved(xpos, ypos); // update camera
	player->setFront(glm::vec3(cam->getFront().x, 0.0, cam->getFront().z));
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// load a texture for a surface
// ----------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true); // stb_image.h to flip loaded texture's on y-axis.
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

// Determine if the camera is close enough to some entity to interact with it
bool is_close_to(glm::vec3 entity_pos)
{
	if(glm::length(cam->getPosition() - entity_pos) <= INTERACT_DISTANCE)
		return true;
	else
		return false;
}

// Collisions
bool collisionAt(glm::vec3 position) 
{
	bool colX = position.x >= WORLD_WIDTH/2 || position.x <= -(WORLD_WIDTH/2);
	bool colZ = position.z >= WORLD_LENGTH/2 || position.z <= -(WORLD_LENGTH/2);

	return colX || colZ;
}

// Add an entity and construct it's model
void addEntity(
	Entity* e, 
	glm::vec3 scales[], glm::vec3 positions[], int numModel,
	unsigned int textures[], int numTextures)	
{
	e->setModel(scales, positions, numModel);
	e->setTextures(textures, numTextures);
	entities.push_back(e);
}

// Add a pickup and construct it's model
void addPickup(
	Pickup* p, 
	glm::vec3 scales[], glm::vec3 positions[], int numModel,
	unsigned int textures[], int numTextures)	
{
	p->setModel(scales, positions, numModel);
	p->setTextures(textures, numTextures);
	pickups.push_back(p);
}

void start()
{	
	//Cleanup (in case of restart)
	delete cam;
	delete player;
	delete tEquip;
	delete table;
	delete walls;
	delete torch;	

	// Container for entities
	entities = std::list<Entity*>();
	pickups = std::list<Pickup*>();

	// Camera	
	cam = new Camera(
		glm::vec3(0.0f, 0.9f, 3.0f), 	// Position
		glm::vec3(0.0f, 0.0f, -1.0f),	// Front face
		glm::vec3(0.0f, 1.0f,  0.0f),	// Up face
 		SCR_WIDTH, SCR_HEIGHT
	);
	entities.push_back(cam);

	// Player
	player = new Entity(
		cam->getPosition(), 
		cam->getFront(),
		cam->getUp()
	);

	// Torch Equip
	tEquip = new Entity( 
		cam->getPosition(),
		cam->getFront(),
		cam->getUp()
	);
	tEquip->setModel(torch_scales, torch_positions, 2);
	tEquip->setTextures(marble_textures, 2);
	cam->setItem(tEquip);
	cam->setItemVisible(false);

	// Table
	table = new Entity(
		glm::vec3(0.0f, 0.0f, -5.0f), 
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	addEntity(table, table_scales, table_positions, 5, wood_textures, 2);

	// Walls
	walls = new Entity(
		glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	addEntity(walls, wall_scales, wall_positions, 4, brick_textures, 2);
	
	// Torch
	torch = new Pickup(
		glm::vec3(cam->getPosition().x, 0.5f, cam->getPosition().z - 3),
		glm::vec3(0.0f, 0.0f, -1.0f),	// Front face
		glm::vec3(0.0f, 1.0f,  0.0f)	// Up face
	);
	addPickup(torch, torch_scales, torch_positions, 2, marble_textures, 2);
	torch->setRotateAnimation(false);
	light_source = torch;
}
