#pragma once
#ifndef Game_h
#define Game_h

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstring>

// BGFX
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
//#include <bgfx/bgfx_utils.h>



// BX
#include <bx/math.h>
#include <bx/allocator.h>
#include <bx/debug.h>

// GLEW
/*
#define GLEW_STATIC
#include <GL/glew.h>
*/
//#include "audio.h"

// GLFW
#include <GLFW/glfw3.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include "GLFW/glfw3native.h"


class Game
{

public:
	Game();
	~Game();

	void init(const char* title, uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, bool fullscreen);

	void handleEvents();
	void update();
	void render();
	void clean();
	void renderPlane(int counter, float xPos, float yPos);

	bool running();
protected:
	bool isRunning;
	GLFWwindow* window;
	bgfx::ProgramHandle program;
	bgfx::VertexBufferHandle vbh;
	bgfx::IndexBufferHandle ibh;
	bgfx::ShaderHandle vsh, fsh;
	int counter = 0;
	float m_xPos = 0.0f;
	float m_yPos = 0.0f;
};

#endif /* Game_h */

