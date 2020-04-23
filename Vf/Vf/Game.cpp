#include "Game.h"

Game::Game() {}
Game::~Game() {}

void Game::init(const char *title, uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, bool fullscreen){

	if (glfwInit()) 
	{
		std::cout << "GLFW Initialized Successfully." << std::endl;
		window = glfwCreateWindow(width, height, title, NULL, NULL);
	}
	else 
	{
		return;
	}

	bgfx::PlatformData pd;
	pd.nwh = glfwGetWin32Window(window);
	bgfx::setPlatformData(pd);
	bgfx::renderFrame();

	if (bgfx::init()) 
	{
		std::cout << "BGFX Initialized Successfully" << std::endl;
		isRunning = true;
	}
	else {
		isRunning = false;
		return;
	}

	bgfx::reset(width, height, BGFX_RESET_VSYNC);
	// Enable debug text.
	bgfx::setDebug(BGFX_DEBUG_STATS /*| BGFX_DEBUG_STATS*/);
	// Set view rectangle for 0th view
	bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));
	// Clear the view rect
	bgfx::setViewClear(0,
		BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
		0x443355FF, 1.0f, 0);

}

void Game::handleEvents()
{
	glfwPollEvents();
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
		bgfx::setViewClear(0,
			BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
			0x344661FF, 1.0f, 0);
	}

	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
		bgfx::setViewClear(0,
			BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
			0x614234FF, 1.0f, 0);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		isRunning = false;
	}
}

void Game::update() {}

void Game::render() {
	bgfx::touch(0);
	bgfx::frame();
}

void Game::clean() {
	bgfx::shutdown();
	glfwDestroyWindow(window);
	glfwTerminate();
	std::cout << "Game Successfully Cleaned." << std::endl;
}

bool Game::running() {
	return isRunning;
}