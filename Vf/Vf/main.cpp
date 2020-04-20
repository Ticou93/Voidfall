#include <stdio.h>
#include <iostream> 
#include <fstream>
// BGFX
#include <bgfx/c99/bgfx.h>
#include <bgfx/platform.h>

// GLEW 
#define GLEW_STATIC 
#include <GL/glew.h> 

// GLFW 
#include <GLFW/glfw3.h> 
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include "audio.h"


// Window dimensions 
const GLint WIDTH = 800, HEIGHT = 600;
uint32_t reset = BGFX_RESET_VSYNC;

int main(void)
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Voidfall x64", NULL, NULL);

	bgfx_platform_data_t pd;
	pd.nwh = glfwGetWin32Window(window);
	bgfx_set_platform_data;

	bgfx_init_t bgfxInit;

	// Automactiaclly choose renderer.
	bgfxInit.type = BGFX_RENDERER_TYPE_COUNT;

	bgfxInit.resolution.width = WIDTH;
	bgfxInit.resolution.height = HEIGHT;
	bgfxInit.resolution.reset = BGFX_RESET_VSYNC;

	// Returns true if init is successful
	bgfx_init;

	// Draw
	bgfx_set_view_clear;
	bgfx_set_view_rect;

	while (true)
		playSound();
		bgfx_frame;

	return 0;
}