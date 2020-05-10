#include "Game.h"
#include "Audio.h"
#include <thread>

Game* game = nullptr;
Audio* audio = nullptr;

void gameThread() {
	game = new Game();
	game->init("Voidfall Alpha", 0, 0, 1200, 720, false);
	while (game->running()) {

		game->handleEvents();
		game->update();
		game->render();
	}
	
	game->clean();
}


void audioThread() {
	audio = new Audio();
	audio->playSound("../sounds/wav/Loonie - Biohazard.wav");
}


int main() {

	std::thread t1(gameThread);
	std::thread t2(audioThread);

	t1.join();
	t2.join();

}

