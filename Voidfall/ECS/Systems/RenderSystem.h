#pragma once

#include "../Core/Coordinator.h"
#include "../Components/Colour.h"
#include "../Components/Renderable.h"
#include "../Core/Types.h"
#include "../Core/System.h"
#include <memory>
#include <iostream>

#include <bx/math.h>
#include <bx/allocator.h>
#include <bgfx/bgfx.h>
//#include "bgfx/bgfx_utils.h"
#include <bgfx/platform.h>
#include "../../tools/shaderHandler.h"

class RenderSystem : public System
{
public:
	void Init(uint16_t width, uint16_t height);
	void Update(float dt);
private:
	Entity mCube;
	bgfx::ProgramHandle program, program1;
	bgfx::VertexBufferHandle vbh, vbh1;
	bgfx::IndexBufferHandle ibh, ibh1;
	bgfx::ShaderHandle vsh, fsh, vsh1, fsh1;
    uint16_t width;
    uint16_t height;
};

