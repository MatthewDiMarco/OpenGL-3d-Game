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
void init(unsigned int textures_wall[]);
void render(unsigned int VAO_box, Shader lighting_shader);
bool is_close_to(glm::vec3 entity_pos);

// Screen
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// World
static const float WORLD_WIDTH = 70.0f;
static const float WORLD_LENGTH = 70.0f;
static const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
static const glm::vec3 NORTH = glm::vec3(0.0f, 0.0f, -1.0f);
static const glm::vec3 ORIGIN = glm::vec3(0.0f, 0.0f, 0.0f);
static const float INTERACT_DISTANCE = 1.6f;
std::list<Entity*> entities;
std::list<Pickup*> pickups;

// Walls
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
Entity walls = Entity(
	glm::vec3(0.0f, 0.0f, 0.0f), 
	glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(0.0f, 1.0f, 0.0f)
);

// Camera
Camera cam = Camera(
	glm::vec3(0.0f, 0.9f, 3.0f), 	// Position
	glm::vec3(0.0f, 0.0f, -1.0f),	// Front face
	glm::vec3(0.0f, 1.0f,  0.0f),	// Up face
 	SCR_WIDTH, SCR_HEIGHT
);

// Player
Entity player = Entity(
	cam.getPosition(), 
	cam.getFront(),
	cam.getUp()
);

// Torch Pickup
Pickup torch = Pickup(
	glm::vec3(cam.getPosition().x, 0.5f, cam.getPosition().z - 3),
	glm::vec3(0.0f, 0.0f, -1.0f),	// Front face
	glm::vec3(0.0f, 1.0f,  0.0f)	// Up face
);

// Torch Equip
Entity tEquip = Entity( 
	cam.getPosition(),
	cam.getFront(),
	cam.getUp()
);

// Table
Entity table = Entity(
	glm::vec3(0.0f, 0.0f, -5.0f), 
	glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(0.0f, 1.0f, 0.0f)
);

// lighting
Entity* light_source; // define what entity is "producing" the light 

// timing
float delta_time = 0.0f;	// time between current frame and last frame
float last_frame = 0.0f;

// Toggle (animation or states)
bool INTERACTIVITY_CLOSE_ENOUGH = false;
int closest_pickup_idx = 0;

// Animation Variables
//float curtin_rotate_y = 0.0;
//float curtin_translate_y = 0.0;

// Determine if the camera is close enough to some entity to interact with it
bool is_close_to(glm::vec3 entity_pos)
{
	if(glm::length(cam.getPosition() - entity_pos) <= INTERACT_DISTANCE)
		return true;
	else
		return false;
}


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
	Shader lamp_shader("./lamp.vs", "./lamp.fs");
	
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

	// Load and create a texture 
	unsigned int tex_wood_diffuse, tex_street_diffuse, tex_grass_diffuse, tex_marble_diffuse, tex_curtin_diffuse;
	unsigned int tex_wood_specular, tex_street_specular, tex_grass_specular, tex_marble_specular, tex_curtin_specular;
	
	tex_wood_diffuse = loadTexture(FileSystem::getPath("resources/textures/wood2.jpg").c_str());
	tex_wood_specular = loadTexture(FileSystem::getPath("resources/textures/wood2_specular.jpg").c_str());
	tex_street_diffuse = loadTexture(FileSystem::getPath("resources/textures/street.png").c_str());
	tex_street_specular = loadTexture(FileSystem::getPath("resources/textures/street_specular.png").c_str());
	tex_grass_diffuse = loadTexture(FileSystem::getPath("resources/textures/grass.jpg").c_str());
	tex_grass_specular = loadTexture(FileSystem::getPath("resources/textures/grass_specular.jpg").c_str());
	tex_marble_diffuse = loadTexture(FileSystem::getPath("resources/textures/marble.jpg").c_str());
	tex_marble_specular = loadTexture(FileSystem::getPath("resources/textures/marble_specular.jpg").c_str());
	tex_curtin_diffuse = loadTexture(FileSystem::getPath("resources/textures/curtin.jpg").c_str());
	tex_curtin_specular = loadTexture(FileSystem::getPath("resources/textures/curtin_specular.jpg").c_str());

	// Initialise WORLD and ENTITIES
	// ------------------------------------------------------------------------------------------

	// Container for entities
	entities = std::list<Entity*>();
	pickups = std::list<Pickup*>();
	
	// Initialise border
	unsigned int wall_tex[] = {tex_wood_diffuse, tex_wood_specular};
	init(wall_tex);

	entities.push_back(&cam);
	
	// Table
	glm::vec3 table2_scales[] = {
		glm::vec3( 1.0f,  0.1f,  1.0f),		//top
		glm::vec3( 0.1f,  0.5f,  0.1f),		//near left
		glm::vec3( 0.1f,  0.5f,  0.1f),		//near right
		glm::vec3( 0.1f,  0.5f,  0.1f),		//far left
		glm::vec3( 0.1f,  0.5f,  0.1f),		//far right
	};
	glm::vec3 table2_positions[] = {
		glm::vec3( 0.0f,  0.5f,  0.0f),		//top
		glm::vec3(-0.45f, 0.2f,  0.45f),	//near left
		glm::vec3( 0.45f, 0.2f,  0.45f),	//near right
		glm::vec3(-0.45f, 0.2f, -0.45f),	//far left
		glm::vec3( 0.45f, 0.2f, -0.45f),	//far right
	};
	addEntity(
		&table, table2_scales, table2_positions, 5, wall_tex, 2
	);

	// Test pickup
	unsigned int pickup_tex[] = {tex_curtin_diffuse, tex_curtin_specular};
	glm::vec3 pickup_scales[] = {
		glm::vec3( 0.2f,  0.2f,  0.2f)
	};
	glm::vec3 pickup_positions[] = {
		glm::vec3( 0.0f,  0.0f,  0.0f)
	};
	Pickup p = Pickup(
		glm::vec3(0.0f, 0.5f, -4.0f), 
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	addPickup(
		&p, pickup_scales, pickup_positions, 1, pickup_tex, 2
	);

	// Torch
	unsigned int torch_tex[] = {tex_marble_diffuse, tex_marble_specular};
	glm::vec3 torch_scales[] = {
		glm::vec3( 0.03f,  0.03f,  0.02f),
		glm::vec3( 0.02f,  0.02f,  0.1f)
	};
	glm::vec3 torch_positions[] = {
		glm::vec3( 0.0f,  0.0f,  0.0f), // Torch head
		glm::vec3( -0.002f,  0.002f,  0.05f) 	// Handle
	};
	torch.setRotateAnimation(false);
	addPickup(
		&torch, torch_scales, torch_positions, 2, torch_tex, 2
	);
	light_source = &torch;

	tEquip.setModel(torch_scales, torch_positions, 2);
	tEquip.setTextures(torch_tex, 2);
	cam.setItem(&tEquip);

	// Shader configuration 
	lighting_shader.use();
	lighting_shader.setInt("material.diffuse", 0);
	lighting_shader.setInt("material.specular", 1);

	// Pass projection matrix to shader 
	// (as projection matrix rarely changes there's no need to do this per frame)
	glm::mat4 projection = glm::perspective(
		glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f
	);
	lighting_shader.setMat4("projection", projection);

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
		lighting_shader.setVec3("light.position", light_source->getPosition());
		lighting_shader.setVec3("light.direction", light_source->getFront());
		lighting_shader.setFloat("light.cutOff", glm::cos(glm::radians(16.5f)));
		lighting_shader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
        lighting_shader.setVec3("viewPos", light_source->getPosition());

		// Light properties
		lighting_shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		lighting_shader.setVec3("light.diffuse", 1.8f, 1.8f, 1.8f);
		lighting_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		lighting_shader.setFloat("light.constant", 0.1f); // brightness
		lighting_shader.setFloat("light.linear", 0.8f);
		lighting_shader.setFloat("light.quadratic", 0.00032f); // view distance

		// Material properties
        lighting_shader.setFloat("material.shininess", 32.0f);
		// for now set the same for every object. But, you can make it dynamic for various obj.

		// Camera/view transformation
		glm::mat4 view = glm::lookAt(
			cam.getPosition(), cam.getPosition() + cam.getFront(), cam.getUp()
		);
		lighting_shader.setMat4("view", view);

		// Declare transformation matrix
		glm::mat4 model = glm::mat4();

		// Draw objects
		// --------------------------------------------------------------------------------------
	
		//Street
		glBindVertexArray(VAO_box); 

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_street_diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_street_specular);

		model = glm::mat4();
		model = glm::scale(model, glm::vec3(10.0f, 0.001f, WORLD_LENGTH));

		lighting_shader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Grass
		glBindVertexArray(VAO_box);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_grass_diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_grass_specular);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -0.01f, 0.0f));
		model = glm::scale(model, glm::vec3(WORLD_WIDTH, 0.001f, WORLD_LENGTH));

		lighting_shader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		// Render the entities
		std::list<Entity*>::iterator it1 = entities.begin();
		for (int ii = 0; ii < (int)(entities.size()); ii++)
		{
			(*it1)->render(VAO_box, lighting_shader);
			std::advance(it1, 1);
		}

		// Render the pickups
		INTERACTIVITY_CLOSE_ENOUGH = false;
		std::list<Pickup*>::iterator it2 = pickups.begin();
		for (int ii = 0; ii < (int)(pickups.size()); ii++)
		{	
			(*it2)->render(VAO_box, lighting_shader);
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
		cam.setSpeed(2.5 * delta_time); 
	else
		cam.setSpeed(2.5 * delta_time * 2);
	
	// Move around
	float cameraSpeed = cam.getSpeed();
	glm::vec3 newPos;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && 
	    !collisionAt(cam.getPosition() + cameraSpeed * player.getFront())) {
		cam.move(cameraSpeed * player.getFront());
		player.move(cameraSpeed * player.getFront());
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS &&
		!collisionAt(cam.getPosition() - cameraSpeed * player.getFront())) {
		cam.move(-cameraSpeed * player.getFront());
		player.move(-cameraSpeed * player.getFront());
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && 
		!collisionAt(cam.getPosition() - cam.getRight() * cameraSpeed)) {
		cam.move(-cam.getRight() * cameraSpeed);
		player.move(-cam.getRight() * cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && 
		!collisionAt(cam.getPosition() + cam.getRight() * cameraSpeed)) {
		cam.move(cam.getRight() * cameraSpeed);
		player.move(cam.getRight() * cameraSpeed);
	}
	
	// Pick stuff up
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && INTERACTIVITY_CLOSE_ENOUGH)
	{
		std::list<Pickup*>::iterator it = pickups.begin();
		std::advance(it, closest_pickup_idx);
		pickups.erase(it);
	
		if ((*it) == (&torch)) {
			cam.setItemVisible(true);
			light_source = &cam;
		}
	}
}

// What to do when the mouse moves 
// ---------------------------------------------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	cam.mouseMoved(xpos, ypos); // update camera
	player.setFront(glm::vec3(cam.getFront().x, 0.0, cam.getFront().z));
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

// Initilaise the world ground, boundaries and basic objects
void init(unsigned int textures_wall[])
{
	// Add the walls to be rendered	
	addEntity(&walls, wall_scales, wall_positions, 4, textures_wall, 2); 
}
