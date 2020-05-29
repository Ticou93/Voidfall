#include "WindowManager.h"
#include "Audio.h"
#include "TaskSystem.h"
#include <random>
#include <chrono>

TaskSystem ts;



WindowManager* wm = nullptr;
RenderSystem* rs = nullptr;
Audio* audio = nullptr;
Coordinator gCoord;

uint16_t width = 800;
uint16_t height = 600;

void videoThread() {
	// Initialize the ECS system coordinator
	gCoord.Init();

	// Init the window 
	wm = new WindowManager();
	wm->Init("Voidfall Alpha", width, height);

	// Register new component
	gCoord.RegisterComponent<Transform>();
	gCoord.RegisterComponent<Faction>();

	// Assign system to component
	auto rs = gCoord.RegisterSystem<RenderSystem>();
	{
		Signature signature;
		signature.set(gCoord.GetComponentType<Transform>());
		signature.set(gCoord.GetComponentType<Faction>());
		gCoord.SetSystemSignature <RenderSystem> (signature);
	}

	// Call system that runs the ECS functions
	rs->Init(width, height, wm->GetWindow());

	std::vector<Entity> entities(MAX_ENTITIES - 1);
	for (auto& entity : entities)
	{
		entity = gCoord.CreateEntity();
		gCoord.AddComponent<Transform>(
			entity,
			{ Vec3(rs->RandomGenerator(1.0f, -1.0f), rs->RandomGenerator(3.0f, 2.0f), 0.0f) }
		);

		gCoord.AddComponent<Faction>(
			entity,
			{ .isEnemy = true }
		);
	}

	// delta time used to rotate cube
	float dt = 0.0f;

	while (wm->running()) {
        wm->update();
		wm->handleEvents();
		rs->Update(dt);
		dt += 0.005f;
	}

	wm->clean();
	return;
}

void audioThread() {
	audio = new Audio();
	audio->playSound("../sounds/wav/ut.wav");
}
int main() {

	std::thread t1(videoThread);
	std::thread t2(audioThread);

	t1.join();
	//t2.join();
	return 0;
}
