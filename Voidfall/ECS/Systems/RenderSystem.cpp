#include "RenderSystem.h"

extern Coordinator gCoord;

Renderable cubeVertices[4];

static const uint16_t cubeTriList[] =
{
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

void RenderSystem::Init(uint16_t width, uint16_t height) {
    this->width = width;
    this->height = height;

	// create entity mCube
	mCube = gCoord.CreateEntity();

	// Create component that covers all the cases of angles for triangles
	gCoord.AddComponent(
		mCube,
		Renderable{
			.xyzc =
			{
				-1.0f,  1.0f,  1.0f, uint32_t(0xFFFFFF),
				 1.0f,  1.0f,  1.0f, uint32_t(0x573461),
				-1.0f, -1.0f,  1.0f, uint32_t(0x573461),

				 1.0f, -1.0f,  1.0f, uint32_t(0x614234),
				-1.0f,  1.0f, -1.0f, uint32_t(0x34613F),
				 1.0f,  1.0f, -1.0f, uint32_t(0x614234),

				-1.0f, -1.0f, -1.0f, uint32_t(0x614234),
				 1.0f, -1.0f, -1.0f, uint32_t(0x34613F),
			}
		}
	);

	bgfx::VertexLayout vl;
	vl.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
		.end();

	for (auto const& entity : mEntities) {
		auto const& mCube = gCoord.GetComponent<Renderable>(entity);
		cubeVertices[entity] = mCube;
	}

	// Background
	bgfx::setViewRect(0, 0, 0, width, height);

	// Clear the view rect
	bgfx::setViewClear(0,
		BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
		0x443355FF, 1.0f, 0);

	vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), vl);
	ibh = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));


	vsh = Shader::loadShader("vs_cubes.bin");
	fsh = Shader::loadShader("fs_cubes.bin");
    std::cout << "Succesfully loaded shaders" << std::endl;
	program = bgfx::createProgram(vsh, fsh, true);
}

void RenderSystem::Update(float dt) {
	const bx::Vec3 at = { 0.0f, 0.0f,  0.0f };
	const bx::Vec3 eye = { 0.0f, 0.0f, -5.0f };
	float view[16];
	bx::mtxLookAt(view, eye, at);
	float proj[16];
	bx::mtxProj(proj, 60.0f, float(this->width-10) / float(this->height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
	bgfx::setViewTransform(0, view, proj);

	bgfx::setVertexBuffer(0, vbh);
	bgfx::setIndexBuffer(ibh);

	bgfx::setViewTransform(0, view, proj);
	float mtx[16];
	bx::mtxRotateXY(mtx, dt * 0.1f, dt * 0.1f);
	bgfx::setTransform(mtx);
	std::cout << dt << std::endl;

	bgfx::submit(0, program);
	bgfx::touch(0);
	bgfx::frame();
}
