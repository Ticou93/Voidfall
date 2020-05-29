#include "RenderSystem.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <random>
#include "../../stb_image.h"




extern Coordinator gCoord;
GLuint texture[4];
GLuint textureLocation;

// Stores the entities Renderable characteristics.
float vertices[] =
{
	// POS					// Texcoords
	 0.5f,  0.5f, 0.0f,		1.0f, 1.0f, // top right
	 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,	// bottom right	
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, // bottom left
	-0.5f,  0.5f, 0.0f,		0.0f, 1.0f // top left 
};

unsigned int indices[] =
{
	0, 1, 3,
	1, 2, 3
};

// Create vector for enemies, create another for player
Shader* enemyShader = nullptr;
Shader* playerShader = nullptr;
GLFWwindow* window = nullptr;
Entity mPlayer;


// Adds a component of type Renderable to the Entity sent via parameter.
Entity RenderSystem::AddComponent() {
	/*
		Assign the Entity Variable with the entity object created from the coordinator,
		thereafter we assign a component (which we registered to a system in main... for now)
		and then return the new entity with it's components to the caller.
	*/
	Entity e{ gCoord.CreateEntity() };
	/*
	Create component that covers all the cases of angles for triangles,
	our cubeTriList will then select the appropriate row to create a set of triangles.
	*/
	gCoord.AddComponent(
		e,
		Transform{
			.position = Vec3(0.0f, 0.0f, 10.0f),
		}
	);

	gCoord.AddComponent(
		e,
		Faction{
			.isEnemy = false
		}
	);
	return e;
}

void RenderSystem::Init(uint16_t width, uint16_t height, GLFWwindow* w) {

	planetPosX += 0.7f;
	planetPosY += 0.5f;
	starsPosY += 0.0f;
	window = w;
	mPlayer = AddComponent();
	this->width = width;
	this->height = height;
	enemyShader = new Shader("shader.vs", "shader.fs");
	InitBuffers();
	InitTexture();
}

float RenderSystem::RandomGenerator(float high, float low) {
	float r = (low) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - (low))));
	return r;
}

void RenderSystem::InitBuffers() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position Attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture coord attrib
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void RenderSystem::InitTexture() {
	int width, height, nrChannels;
	glGenTextures(3, texture);
	glBindTexture(GL_TEXTURE_2D, texture[0]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture 2
	// ---------
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("ship.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	

	// texture 3
	// ---------
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("space.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture 4
	// ---------
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("planet.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	enemyShader->use();
	enemyShader->setInt("texture", 0);
	
}

 void RenderSystem::Update(float dt) {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	auto const& playerTransform = gCoord.GetComponent<Transform>(mPlayer);

	// backdrop Draw
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0.0f, starsPosY -= 0.00008f, 0.0f));
	trans = glm::scale(trans, glm::vec3(4.0f, 4.0f, 2.0f));
	transformLoc = glGetUniformLocation(enemyShader->ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Scene Draw
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(planetPosX -= 0.000005f, planetPosY -= 0.000025, 0.0f));
	trans = glm::scale(trans, glm::vec3(2.0f, 2.0f, 1.0f));
	transformLoc = glGetUniformLocation(enemyShader->ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans[0][0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Draw Enemies
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	for (auto const& entity : mEntities) 
	{
		auto& enemyTransform = gCoord.GetComponent<Transform>(entity);
		auto& faction = gCoord.GetComponent<Faction>(entity);

		if(faction.isEnemy)
		{
			glm::mat4 trans = glm::mat4(1.0f);
			trans = glm::translate(trans, glm::vec3(enemyTransform.position.x, enemyTransform.position.y, 0.0f));
			trans = glm::rotate(trans, (dt)*8, glm::vec3(0.0f, 0.0f, 1.0f));
			trans = glm::scale(trans, glm::vec3(0.2f, 0.2f, 1.0f));
			transformLoc = glGetUniformLocation(enemyShader->ID, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans[0][0]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			if ((float)enemyTransform.position.y < -1.0f) 
			{
				enemyTransform.position.x = RandomGenerator(1.0f, -1.0f);
				enemyTransform.position.y = RandomGenerator(1.0f, 1.0f);
			};
			enemyTransform.position.y -= 0.01f;
			BasicCollision(entity, mPlayer);
		}
	}
	
	// Player draw
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(playerTransform.position.x, playerTransform.position.y , 0.0f));
	trans = glm::rotate(trans, 3.142f, glm::vec3(0.0f, 0.0f, 1.0f));
	trans = glm::scale(trans, glm::vec3(0.2f, 0.3f, 1.0f));
	transformLoc = glGetUniformLocation(enemyShader->ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans[0][0]);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	PlayerControls(window);
}

 void RenderSystem::BasicCollision(Entity enemy, Entity player)
 {
	 float offset = 0.1f;
	 auto& enemyTransform = gCoord.GetComponent<Transform>(enemy);
	 auto& playerTransform = gCoord.GetComponent<Transform>(player);
	 if ((enemyTransform.position.y - offset <= playerTransform.position.y + offset) &&
		 (enemyTransform.position.y + offset >= playerTransform.position.y - offset) &&
		 (enemyTransform.position.x - offset <= playerTransform.position.x + offset) &&
		 (enemyTransform.position.x + offset >= playerTransform.position.x - offset))
	 {
		 glfwTerminate();
		 std::cout << "GAME OVER" << std::endl;
	 }
 }

 void RenderSystem::PlayerControls(GLFWwindow* w) 
 {
	 auto& playerTransform = gCoord.GetComponent<Transform>(mPlayer);
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	 {
		 if (playerTransform.position.x > -0.93f)
			 playerTransform.position.x -= 0.007f;
	 }
	 else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	 {
		 if(playerTransform.position.x < 0.93f)
			playerTransform.position.x += 0.007f;
	 }

	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
	 {
		 if(playerTransform.position.y < 0.93f)
			playerTransform.position.y += 0.007f;
	 }
	 else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
	 {
		 if (playerTransform.position.y > -0.93f)
			 playerTransform.position.y -= 0.007f;
	 }
 }
