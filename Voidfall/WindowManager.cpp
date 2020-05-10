#include "WindowManager.h"
#include "ECS/Core/Coordinator.h"
#include "ECS/Components/Colour.h"
#include "ECS/Systems/RenderSystem.h"
#include <random>


extern Coordinator gCoord;

WindowManager::WindowManager() {}
WindowManager::~WindowManager() {}

// Initializes the render window with GLFW and BGFX
void WindowManager::Init(const char* title, uint16_t _width, uint16_t _height) {

	if (glfwInit())
	{
		std::cout << "GLFW Initialized Successfully." << std::endl;
		window = glfwCreateWindow(_width, _height, title, NULL, NULL);
	}
	else
	{
		return;
	}

	debug = BGFX_DEBUG_NONE;
	reset = BGFX_RESET_VSYNC;

	

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	pd.ndt = glfwGetX11Display();
	pd.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_WINDOWS
	pd.nwh = glfwGetWin32Window(window);
#elif BX_PLATFORM_OSX
	pd.nwh = glfwGetCocoaWindow(window);
#endif	
	

	// Set data on which platform it is that runs bgfx.
	bgfx::setPlatformData(pd);
	bgfx::renderFrame();

	// Initialize bgfx
	bgfx::Init init;
	init.resolution.width = _width;
	init.resolution.height = _height;


	if (bgfx::init(init))
	{
		std::cout << "BGFX Initialized Successfully" << std::endl;
		isRunning = true;
	}
	else {
		isRunning = false;
		return;
	}
}


void WindowManager::handleEvents()
{
	glfwPollEvents();
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		isRunning = false;
	}
}

void WindowManager::update() {}

void WindowManager::clean() {
	bgfx::shutdown();
	glfwDestroyWindow(window);
	glfwTerminate();
	std::cout << "Window Successfully Cleaned." << std::endl;
}

bool WindowManager ::running() {
	return isRunning;
}