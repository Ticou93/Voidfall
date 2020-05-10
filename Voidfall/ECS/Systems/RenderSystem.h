#pragma once

#include "../Core/Types.h"
#include "../Core/System.h"
#include <memory>
#include "bgfx/bgfx.h"
#include "bgfx/bgfx_utils.h"
#include "bgfx/platform.h"

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

