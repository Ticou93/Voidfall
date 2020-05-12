#include "WindowManager.h"

extern Coordinator gCoord;

WindowManager::WindowManager() {}
WindowManager::~WindowManager() {}

// Initializes the render window with GLFW and BGFX
void WindowManager::Init(const char* title, uint16_t width, uint16_t height) {

	if (glfwInit())
	{
		std::cout << "GLFW Initialized Successfully." << std::endl;
		window = glfwCreateWindow(width, height, title, NULL, NULL);
	}
	else
	{
		return;
	}

	//debug = BGFX_DEBUG_NONE;
	//debug = BGFX_DEBUG_TEXT;
	debug = BGFX_DEBUG_TEXT | BGFX_DEBUG_STATS;
	reset = BGFX_RESET_VSYNC;

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    pd.ndt = glfwGetX11Display();
    pd.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_WINDOWS
	pd.nwh = glfwGetWin32Window(window);
#elif BX_PLATFORM_OSX
	pd.nwh = glfwGetCocoaWindow(window);
#endif
    pd.context = NULL;
    pd.backBuffer = NULL;
    pd.backBufferDS = NULL;

	// Initialize bgfx
	bgfx::Init init;
	init.resolution.width = width;
	init.resolution.height = height;
    init.resolution.reset = reset;
    init.platformData = pd;

	bgfx::renderFrame();

	if (bgfx::init(init))
	{
		std::cout << "BGFX Initialized Successfully" << std::endl;
		isRunning = true;
	}
	else {
		isRunning = false;
		return;
	}

	bgfx::setDebug(debug);
}


void WindowManager::handleEvents()
{
	glfwPollEvents();
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		isRunning = false;
	}
}

void WindowManager::update() {
}

void WindowManager::clean() {
	bgfx::shutdown();
	glfwDestroyWindow(window);
	glfwTerminate();
	std::cout << "Window Successfully Cleaned." << std::endl;
}

bool WindowManager ::running() {
	return isRunning;
}
