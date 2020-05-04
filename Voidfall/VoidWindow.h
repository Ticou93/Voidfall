#pragma once
#include <stdio.h>
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "bx/math.h"
#include "GLFW/glfw3.h"

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include "GLFW/glfw3native.h"

class VoidWindow{
public:
    VoidWindow() : window(nullptr){}

    bool Init(uint32_t a_width = 800, uint32_t a_height = 640){
        if (glfwInit()== GLFW_FALSE) return false;//Leave on glfw init failure
        GLFWwindow* window = glfwCreateWindow(a_width, a_height, "VoidEngine", nullptr, nullptr);

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

        bgfx::Init bgfxInit;
        bgfxInit.type = bgfx::RendererType::Count;
        bgfxInit.resolution.width = a_width;
        bgfxInit.resolution.height = a_height;
        bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
        if (!bgfx::init(bgfxInit)) {//Did we fail? If so delete any resources and leave
            glfwDestroyWindow(window);
            window = nullptr;
            return false;
        }
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xF43355FF, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, a_width, a_height);
        return true;
	}

    ~VoidWindow() {
        bgfx::shutdown();
        glfwDestroyWindow(window);
        glfwTerminate();
    }
protected:
    GLFWwindow* window;
};
