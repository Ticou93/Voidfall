#include "WindowManager.h"
#include "Audio.h"
#include "ECS/Core/Coordinator.h"
#include "ECS/Components/Renderable.h"
#include "ECS/Systems/RenderSystem.h"

WindowManager* wm = nullptr;
Audio* audio = nullptr;
Coordinator gCoord;


void videoThread() {

	// Initialize the ECS system coordinator
	gCoord.Init();

	// Register new component
	gCoord.RegisterComponent<Renderable>();

	wm = new WindowManager();
	wm->Init("Voidfall Alpha", 1200, 720);

	// Assign system to component
	auto renderSystem = gCoord.RegisterSystem<RenderSystem>();
	{
	
		Signature signature;
		signature.set(gCoord.GetComponentType<Renderable>());
	}
	
	// Call system that runs the ECS functions
	renderSystem->Init();

	// delta time used to rotate cube
	float dt = 0.0f;

	while (wm->running()) {
		wm->handleEvents();
		renderSystem->Update(dt);
		dt += 0.05f;
	}

	wm->clean();
	return;
}

void audioThread() {
	audio = new Audio();
}

int main() {

	std::thread t1(videoThread);
	std::thread t2(audioThread);

	t1.join();
	t2.join();

}
