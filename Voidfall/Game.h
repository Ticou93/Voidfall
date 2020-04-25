#pragma once
#ifndef Game_h
#define Game_h

//#include "audio.h"

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <fstream>
// BGFX
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

// BX
#include <bx/math.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

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

	bool running();
protected:
	bool isRunning;
	GLFWwindow* window;


};

#endif /* Game_h */

