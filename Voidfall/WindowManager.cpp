#include "WindowManager.h"

extern Coordinator gCoord;

WindowManager::WindowManager() {}
WindowManager::~WindowManager() {}



// Initializes the render window with GLFW and BGFX
void WindowManager::Init(const char* title, uint16_t width, uint16_t height) {

	if (glfwInit())
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		std::cout << "GLFW successfully initialized." << std::endl;
		window = glfwCreateWindow(width, height, title, NULL, NULL);
		glfwMakeContextCurrent(window);
	}
	else
	{
		return;
	}

#/*if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    pd.ndt = glfwGetX11Display();
    pd.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_WINDOWS
	glfwGetWin32Window(window);
#elif BX_PLATFORM_OSX
	pd.nwh = glfwGetCocoaWindow(window);

#endif*/


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD." << std::endl;
		isRunning = false;
		return;
	}
	else {
		std::cout << "GLAD successfully initialized." << std::endl;
		isRunning = true;
	}

	glfwMakeContextCurrent(window);
	glViewport(0, 0, 800, 600);

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void WindowManager::handleEvents()
{
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		isRunning = false;
	}
}

void WindowManager::update() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void WindowManager::clean() {
	glfwDestroyWindow(window);
	glfwTerminate();
	std::cout << "Window Successfully Cleaned." << std::endl;
}

bool WindowManager::running() {
	return isRunning;
}

GLFWwindow* WindowManager::GetWindow() {
	return window;
}
