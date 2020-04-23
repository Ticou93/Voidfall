#include <stdio.h>
#include <iostream>
#include <fstream>

// BGFX
//#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
//#include "logo.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
uint32_t reset = BGFX_RESET_VSYNC;

int main(void)
{
	glfwInit();
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Hello, bgfx!", nullptr, nullptr);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	if (!window)
		return 1;
	//glfwSetKeyCallback(window, glfw_keyCallback);

    bgfx::renderFrame();

    bgfx::Init init;
    #if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	init.platformData.ndt = glfwGetX11Display();
	init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
    #elif BX_PLATFORM_OSX
	init.platformData.nwh = glfwGetCocoaWindow(window);
    #elif BX_PLATFORM_WINDOWS
	init.platformData.nwh = glfwGetWin32Window(window);
    #endif

	//bgfx_set_platform_data;

	//bgfx_init_t bgfxInit;

	// Automactiaclly choose renderer.
	//init.type = BGFX_RENDERER_TYPE_COUNT;

	init.resolution.width = WIDTH;
	init.resolution.height = HEIGHT;
	init.resolution.reset = BGFX_RESET_VSYNC;

	// Returns true if init is successful
    if (!bgfx::init(init))
        return 1;

    const bgfx::ViewId kClearView = 0;
	bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR);
	bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

	while(true)
		bgfx::frame();

    bgfx::shutdown();
	return 0;
}
