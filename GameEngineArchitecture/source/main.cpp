#define USE_GLFW 1
#ifdef USE_GLFW
#include "GLFW_EngineCore.h"
#endif

#include <memory>

#include "Game.h"
#include "DefaultGame.h"

#include "MemoryManager.h"

int main(int p_Argc, char* p_Argv[]) {
	MemoryManagerInstance;

	std::shared_ptr<IEngineCore> engineCore;

#ifdef USE_GLFW
	 engineCore = std::make_shared<GLFW_EngineCore>();
#endif

	if (!engineCore->InitWindow(800, 600, "GameEngine"))
		return -1;

	std::shared_ptr<Game> game = std::make_shared<DefaultGame>();
	engineCore->RunEngine(game);

	return 0;
}