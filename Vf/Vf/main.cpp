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

#include "audio.h"

// GLFW 
#include <GLFW/glfw3.h> 
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"




// Window dimensions 
#define WNDW_WIDTH 1600
#define WNDW_HEIGHT 900




int main() {

	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WNDW_WIDTH, WNDW_HEIGHT, "Hello, bgfx!", NULL, NULL);
	

	bgfx::PlatformData pd;
	pd.nwh = glfwGetWin32Window(window);
	bgfx::setPlatformData(pd);
	bgfx::renderFrame();

	bgfx::init();

	bgfx::reset(WNDW_WIDTH, WNDW_HEIGHT, BGFX_RESET_VSYNC);

	bgfx::setDebug(BGFX_DEBUG_TEXT /*| BGFX_DEBUG_STATS*/);

    // Enable debug text.
    bgfx::setDebug(BGFX_DEBUG_TEXT /*| BGFX_DEBUG_STATS*/);

    // Set view rectangle for 0th view
    bgfx::setViewRect(0, 0, 0, uint16_t(WNDW_WIDTH), uint16_t(WNDW_HEIGHT));

    // Clear the view rect
    bgfx::setViewClear(0,
        BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
        0x443355FF, 1.0f, 0);

    // Set empty primitive on screen
    bgfx::touch(0);
    

    // Currently the frame gets rendered but the sound buffer takes over and plays the track without updating the graphics
    while(true){
        bgfx::frame();
        playSound("../testy.wav");
    }
      


    bgfx::shutdown();
    
}

