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
//#include "../../shader.h"
#include "../../tools/shaderHandler.h"


//#include "../Core/Coordinator.h"
//#include "bgfx/bgfx.h"
//#include "bgfx/bgfx_utils.h"
//#include "bgfx/platform.h"
//#include "../Components/Colour.h"
//#include "../Core/Coordinator.h"
//#include "../../tools/shaderHandler.h"
//#include "../Components/Renderable.h"
//#include <iostream>

class RenderSystem : public System
{
public:
	void Init();

	void Update(float dt);

private:

	Entity mCube;
	bgfx::ProgramHandle program, program1;
	bgfx::VertexBufferHandle vbh, vbh1;
	bgfx::IndexBufferHandle ibh, ibh1;
	bgfx::ShaderHandle vsh, fsh, vsh1, fsh1;
};

