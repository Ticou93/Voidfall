#include "Game.h"

Game::Game() {}
Game::~Game() {}


struct PosColorVertex{
	float x;
	float y;
	float z;
	uint32_t abgr;
};

static PosColorVertex cubeVertices[] ={
	{-1.0f,  1.0f,  1.0f, 0xff000000 },
	{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
	{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
	{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
	{-1.0f,  1.0f, -1.0f, 0xffff0000 },
	{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
	{-1.0f, -1.0f, -1.0f, 0xffffff00 },
	{ 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t cubeTriList[] ={
	0, 1, 2,
	1, 3, 2,
	4, 6, 5,
	5, 6, 7,
	0, 2, 4,
	4, 2, 6,
	1, 5, 3,
	5, 7, 3,
	0, 4, 1,
	4, 5, 1,
	2, 3, 6,
	6, 3, 7,
};

void Game::init(const char *title, uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, bool fullscreen){

	if (!glfwInit()) return;

	std::cout << "GLFW Initialized Successfully." << std::endl;
	window = glfwCreateWindow(width, height, title, nullptr, nullptr);

	bgfx::PlatformData pd;

	#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	pd.ndt = glfwGetX11Display();
	pd.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
	#elif BX_PLATFORM_WINDOWS
	pd.nwh = glfwGetWin32Window(window);
	#elif BX_PLATFORM_OSX
	pd.nwh = glfwGetCocoaWindow(window);
	#endif
	bgfx::setPlatformData(pd);
	bgfx::renderFrame();

	if (bgfx::init()) {
		std::cout << "BGFX Initialized Successfully" << std::endl;
		isRunning = true;
	} else {
		isRunning = false;
		return;
	}

	bgfx::reset(width, height, BGFX_RESET_VSYNC);
	// Enable debug text.
	bgfx::setDebug(BGFX_DEBUG_STATS /*| BGFX_DEBUG_STATS*/);
	// Set view rectangle for 0th view
	bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));
	// Clear the view rect
	bgfx::setViewClear(0,BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,0x443355FF, 1.0f, 0);


	bgfx::VertexLayout vl;
	vl.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
		.end();
	vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), vl);
	ibh = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));

	vsh = loadShader("vs_cubes.bin");
	fsh = loadShader("fs_cubes.bin");
	program = bgfx::createProgram(vsh, fsh, true);
}

void Game::handleEvents(){
	glfwPollEvents();
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
		bgfx::setViewClear(0,BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,0x344661FF, 1.0f, 0);
	}

	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
		bgfx::setViewClear(0,BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,0x614234FF, 1.0f, 0);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		isRunning = false;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		m_yPos -= 0.1;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		m_xPos += 0.1;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		m_yPos += 0.1;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		m_xPos -= 0.1;
	}
}

void Game::update() {}

void Game::render() {
	std::cout << m_xPos << std::endl;
	renderPlane(counter++, m_xPos, m_yPos);
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

void Game::renderPlane(int counter, float xPos, float yPos) {
	const bx::Vec3 at = { xPos, yPos,  0.0f };
	const bx::Vec3 eye = { 0.0f, 0.0f, -10.0f };
	float view[16];
	bx::mtxLookAt(view, eye, at);
	float proj[16];
	bx::mtxProj(proj, 60.0f, float(1200) / float(720), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
	bgfx::setViewTransform(0, view, proj);

	bgfx::setVertexBuffer(0, vbh);
	bgfx::setIndexBuffer(ibh);

	bgfx::setViewTransform(0, view, proj);
	float mtx[16];
	bx::mtxRotateXY(mtx, counter * 0.01f, counter * 0.01f);
	bgfx::setTransform(mtx);

	bgfx::submit(0, program);
}
