#pragma once

// BGFX
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
//#include <bgfx/bgfx_utils.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstring>

namespace Shader {
	// Handles shaders
	bgfx::ShaderHandle loadShader(const char* FILENAME);
}
