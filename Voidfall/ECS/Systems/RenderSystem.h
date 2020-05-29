#pragma once

#include "../Core/Coordinator.h"
#include "../Core/Types.h"
#include "../Core/System.h"
#include <memory>
#include <iostream>
#include "../../math/Vec3.h"
#include "../../Shader.h"

// ECS Components
#include "../Components/Transform.h"
#include "../Components/Faction.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class RenderSystem : public System
{
public:
	void Init(uint16_t width, uint16_t height, GLFWwindow* w);
	void Update(float dt);
	
	float RandomGenerator(float high, float low);

protected:
	void PlayerControls(GLFWwindow* w);
private:
	Entity AddComponent();
	void BasicCollision(Entity enemy, Entity player);
	void InitTexture();
	void InitBuffers();
	uint16_t width;
	uint16_t height;
	float playerPosX, playerPosY, planetPosX, planetPosY, starsPosY;
	

	unsigned int VAO, VBO, EBO, shaderProgram, fragmentShader, vertexShader, transformLoc;
};

