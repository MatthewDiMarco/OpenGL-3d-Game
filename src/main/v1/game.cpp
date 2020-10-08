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

#define PI 3.14159265

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
void addEntity(Entity e, bool solid); 
void addEntity(
	Entity e, 
	glm::vec3 scales[], glm::vec3 positions[], int numModel,
	unsigned int textures[], int numTextures, bool solid);
void init(unsigned int textures_wall[]);
void render(unsigned int VAO_box, Shader lighting_shader);

// Screen
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// World
static const float WORLD_WIDTH = 70.0f;
static const float WORLD_LENGTH = 70.0f;
static const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
static const glm::vec3 NORTH = glm::vec3(0.0f, 0.0f, -1.0f);
static const glm::vec3 ORIGIN = glm::vec3(0.0f, 0.0f, 0.0f);
std::list<Entity> entities;

// Walls
Entity walls = Entity(
	glm::vec3(0.0f, 0.0f, 0.0f), 
	glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(0.0f, 1.0f, 0.0f)
);

// Temp Entity
Entity table2 = Entity(
	glm::vec3(10.0f, 0.0f, 10.0f), 
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
	cam.getPosition(),				// Position 
	cam.getUp(), 					// Up face
	glm::vec3(0.0f, 0.0f, -1.0f) 	// track player's front w/o upwards direction of camera
);

// lighting
glm::vec3 light_pos(0.0f, 1.0f, 0.1f);

// timing
float delta_time = 0.0f;	// time between current frame and last frame
float last_frame = 0.0f;

// Toggle (animation or states)
bool BUTTON_PRESSED = false;
int BUTTON_DELAY = 0;
bool BUTTON_CLOSE_ENOUGH = false;

int SHOW_DELAY = 0;

// Animation Variables
float curtin_rotate_y = 0.0;
float curtin_translate_y = 0.0;

// Countdown until the button trigger can be pressed again.
// This prevents accidental burst repeat clicking of the key.
void update_delay()
{
	if(BUTTON_DELAY > 0) BUTTON_DELAY -= 1;
	if(SHOW_DELAY > 0) SHOW_DELAY -= 1;
}

// Toggle button pressing only if the camera is close enough.
void toggle_button_distance(glm::vec3 button_pos)
{
	if(glm::length(cam.getPosition() - button_pos) <= 1.6f)
		BUTTON_CLOSE_ENOUGH = true;
	else
		BUTTON_CLOSE_ENOUGH = false;
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
	unsigned int tex_red_dark_diffuse, tex_red_bright_diffuse;
	unsigned int tex_red_dark_specular, tex_red_bright_specular;

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

	tex_red_dark_diffuse = loadTexture(FileSystem::getPath("resources/textures/red_dark.jpg").c_str());
	tex_red_dark_specular = loadTexture(FileSystem::getPath("resources/textures/red_dark_specular.jpg").c_str());
	tex_red_bright_diffuse = loadTexture(FileSystem::getPath("resources/textures/red_bright.jpg").c_str());
	tex_red_bright_specular = loadTexture(FileSystem::getPath("resources/textures/red_bright_specular.jpg").c_str());

	// Initialise WORLD and ENTITIES
	// ------------------------------------------------------------------------------------------

	entities = std::list<Entity>();
	
	unsigned int wall_tex[] = {tex_wood_diffuse, tex_wood_specular};
	init(wall_tex);

	// TABLE 2 EXAMPLE
	glm::vec3 table2_scales[] = {
		glm::vec3( 1.0f,  0.1f,  1.0f),		//top
		glm::vec3( 0.1f,  0.5f,  0.1f),		//near left
		glm::vec3( 0.1f,  0.5f,  0.1f),		//near right
		glm::vec3( 0.1f,  0.5f,  0.1f),		//far left
		glm::vec3( 0.1f,  0.5f,  0.1f),		//far right
	};
	glm::vec3 table2_positions[] = {
		glm::vec3( 0.0f,  0.5f,  0.0f),		//top
		glm::vec3(-0.45f, 0.0f,  0.45f),	//near left
		glm::vec3( 0.45f, 0.0f,  0.45f),	//near right
		glm::vec3(-0.45f, 0.0f, -0.45f),	//far left
		glm::vec3( 0.45f, 0.0f, -0.45f),	//far right
	};
	table2.setModel(table2_scales, table2_positions, 5);
	table2.setTextures(wall_tex, 2);
	addEntity(table2, true);
	
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

		// Update delay countdown
		update_delay();

		// Input
		process_input(window);

		// Render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		// Activate shader
		lighting_shader.use();
		lighting_shader.setVec3("light.position", light_pos);
        lighting_shader.setVec3("viewPos", cam.getPosition());

		// Light properties
		lighting_shader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);

		if(BUTTON_PRESSED == true)
		{
			lighting_shader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
			lighting_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		}
		else
		{
			lighting_shader.setVec3("light.diffuse", 0.0f, 0.0f, 0.0f);
			lighting_shader.setVec3("light.specular", 0.0f, 0.0f, 0.0f);
		}

		// Material properties
        lighting_shader.setFloat("material.shininess", 65.0f);
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
		std::list<Entity>::iterator it = entities.begin();
		for (int ii = 0; ii < (entities.size()); ii++)
		{
			((Entity)(*it)).render(VAO_box, lighting_shader);
			std::advance(it, 1);
		}
	
		// Button on table (1 big box & 1 small box as button)
		glm::vec3 button_scales[] = {
			glm::vec3( 0.2f,  0.12f,  0.2f),	//case
			glm::vec3( 0.12f,  0.12f,  0.12f),	//button
		};

		float red_button_height = 0.05f;
		if(BUTTON_PRESSED == true) {red_button_height -= 0.02f;}

		glm::vec3 button_positions[] = {
			glm::vec3( 0.0f,  0.0f,  0.0f),					//case
			glm::vec3( 0.0f,  red_button_height,  0.0f),	//button
		};

		glm::vec3 button_final_location = glm::vec3(0.0f, 0.56f, 0.25f);
		toggle_button_distance(button_final_location); 

		glBindVertexArray(VAO_box);
		
		for(int tab = 0; tab < 2; tab++)
		{	
			glActiveTexture(GL_TEXTURE0);
			if(tab == 0)
			{	
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex_marble_diffuse);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, tex_marble_specular);
			}
			else
			{
				if(BUTTON_PRESSED == false) // Not Pressed
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, tex_red_dark_diffuse);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, tex_red_dark_specular);
				}
				else // Pressed
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, tex_red_bright_diffuse);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, tex_red_bright_specular);
				}
			}

			model = glm::mat4();
			model = glm::translate(model, button_final_location);
			model = glm::translate(model, button_positions[tab]);
			model = glm::scale(model, button_scales[tab]);
			model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));

			lighting_shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Curtin Logo
		glBindVertexArray(VAO_box);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_curtin_diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_curtin_specular);

		// Transformation for animation
		if(BUTTON_PRESSED == true)
		{
			curtin_translate_y += 6.0f;
			curtin_rotate_y += 6.0f;
			if(abs(curtin_translate_y - 360.0f) <= 0.1f) curtin_translate_y = 0.0f;
			if(abs(curtin_rotate_y - 360.0f) <= 0.1f) curtin_rotate_y = 0.0f;
		}

		model = glm::mat4();
		model = glm::translate(
			model, 
			glm::vec3(0.0f, 0.9f + (0.1f * sin(curtin_translate_y * PI / 180.f)), -0.35f)	
		);
		model = glm::rotate(model, glm::radians(curtin_rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.001f));

		lighting_shader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Draw the light source
		lamp_shader.use(); 
		lamp_shader.setMat4("projection", projection);
		lamp_shader.setMat4("view", view);
		model = glm::mat4();
		model = glm::translate(model, light_pos);
		model = glm::scale(model, glm::vec3(0.01f)); // a smaller cube
		lamp_shader.setMat4("model", model);
		
		if(BUTTON_PRESSED == true) lamp_shader.setFloat("intensity", 1.0);
		else lamp_shader.setFloat("intensity", 0.3);

		glBindVertexArray(VAO_light);
		glDrawArrays(GL_TRIANGLES, 0, 36);



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

	float cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		cameraSpeed = 2.5 * delta_time; 
	else
		cameraSpeed = 2.5 * delta_time * 2;	// double speed with "Shift" pressed

	// Move around
	glm::vec3 newPos;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && 
	    !collisionAt(cam.getPosition() + cameraSpeed * player.getFront())) {
		cam.setPosition(cam.getPosition() + cameraSpeed * player.getFront());
		player.setPosition(cam.getPosition() + cameraSpeed * player.getFront());
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS &&
		!collisionAt(cam.getPosition() - cameraSpeed * player.getFront())) {
		cam.setPosition(cam.getPosition() - cameraSpeed * player.getFront());
		player.setPosition(player.getPosition() - cameraSpeed * player.getFront());
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && 
		!collisionAt(cam.getPosition() - cam.getRight() * cameraSpeed)) {
		cam.setPosition(cam.getPosition() - cam.getRight() * cameraSpeed);
		player.setPosition(player.getPosition() - cam.getRight() * cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && 
		!collisionAt(cam.getPosition() + cam.getRight() * cameraSpeed)) {
		cam.setPosition(cam.getPosition() + cam.getRight() * cameraSpeed);
		player.setPosition(player.getPosition() + cam.getRight() * cameraSpeed);
	}

	// Toggle red button
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && 
		BUTTON_DELAY == 0 && 
		BUTTON_CLOSE_ENOUGH == true)
	{
		BUTTON_DELAY = 20;
		if(BUTTON_PRESSED == false) 		
			BUTTON_PRESSED = true;
		else
			BUTTON_PRESSED = false;
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
 	//std::cout << position.x << "  " << colX << std::endl;
 	//std::cout << position.z << "  " << colZ << std::endl << std::endl;

	return colX || colZ;
}

// Add an entity 
void addEntity(Entity e, bool solid) 
{
	e.setSolid(solid);
	entities.push_back(e);
}

// Add an entity and construct it's model
void addEntity(
	Entity e, 
	glm::vec3 scales[], glm::vec3 positions[], int numModel,
	unsigned int textures[], int numTextures, bool solid)	
{
	e.setModel(scales, positions, numModel);
	e.setTextures(textures, numTextures);
	e.setSolid(solid);
	entities.push_back(e);
}

// Initilaise the world ground, boundaries and basic objects
void init(unsigned int textures_wall[])
{
	//Walls
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
			
	// Add the walls to be rendered	
	walls.setModel(wall_scales, wall_positions, 4);
	walls.setTextures(textures_wall, 2);
	addEntity(walls, false);	
}
