#pragma once
#ifndef Game_h
#define Game_h

#define _CRT_SECURE_NO_DEPRECATE
#define ENTRY_IMPLEMENT_MAIN_1

// Default c++ headers
#include <stdio.h>
#include <iostream>

// BGFX
#include "bgfx/platform.h"
//#include "common.h"

// BIMG
#include "bimg/decode.h"

// BX
#include "bx/math.h"
#include <bx/uint32_t.h>

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

// Other

//#include "bgfx/bgfx_utils.h"


class WindowManager
{

public:
	WindowManager();
	~WindowManager();

	void Init(const char* title, uint16_t _width, uint16_t _height);

	void handleEvents();
	void update();
	void render();
	void clean();
	void componentGenerator();

	bool running();

protected:

	bool isRunning;
	GLFWwindow* window;
	bgfx::PlatformData pd;
	bgfx::ProgramHandle program;
	bgfx::VertexBufferHandle vbh;
	bgfx::IndexBufferHandle ibh;
	bgfx::ShaderHandle vsh, fsh;
	bgfx::TextureHandle mtc;
	bgfx::UniformHandle stc;
	float m_xPos;
	float m_yPos;
	uint32_t reset;
	uint32_t debug;

};

#endif /* Game_h */

