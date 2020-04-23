#include "Game.h"

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
	return;
}

void audioThread() {
	audio = new Audio();
	audio->playSound("../testy.wav");
}

int main() {

	std::thread t1(gameThread);
	std::thread t2(audioThread);

	t1.join();
	t2.join();

}

