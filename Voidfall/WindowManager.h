#pragma once

#define _CRT_SECURE_NO_DEPRECATE
#define ENTRY_IMPLEMENT_MAIN_1

// Default c++ headers
#include <stdio.h>
#include <random>
#include "ECS/Systems/RenderSystem.h"

// GLAD
#include "glad/glad.h"

// GLFW
#include <GLFW/glfw3.h>
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

class WindowManager
{

public:
	WindowManager();
	~WindowManager();

	void Init(const char* title, uint16_t _width, uint16_t _height);

	void handleEvents();
	void update();
	GLFWwindow* GetWindow();
	void clean();
	void componentGenerator();
	RenderSystem AssignRenderer(RenderSystem renderer);

	bool running();

protected:

	bool isRunning;
	GLFWwindow* window;
	float m_xPos;
	float m_yPos;
	uint32_t reset;
	uint32_t debug;

};

