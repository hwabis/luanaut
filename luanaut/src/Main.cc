#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <spdlog/spdlog.h>
#include "Game.h"
#include "MeshNode.h"

// todo all these SDL_ init methods needs to be abstracted out of the app lol
// maybe some weird macro thing

class MyAwesomeScene : public luanaut::Scene {
 public:
  auto Load() -> void override {
    AddChild(std::make_unique<luanaut::MeshNode>());
  }
};

class MyAwesomeGame : public luanaut::Game {
 public:
  MyAwesomeGame() : Game(std::make_unique<MyAwesomeScene>()) {}
};

auto SDL_AppInit(void** appState, int /*argc*/, char** /*argv*/)
    -> SDL_AppResult {
  if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO)) {
    spdlog::error(SDL_GetError());
    return SDL_APP_FAILURE;
  }

  auto* game = new MyAwesomeGame();
  game->Init();  // Not a smell 👍
  *appState = game;

  return SDL_APP_CONTINUE;
}

auto SDL_AppIterate(void* appState) -> SDL_AppResult {
  auto* game = static_cast<luanaut::Game*>(appState);

  if (!game->IsAlive()) {
    return SDL_APP_SUCCESS;
  }

  game->UpdateSubTree();

  return SDL_APP_CONTINUE;
}

auto SDL_AppEvent(void* appState, SDL_Event* event) -> SDL_AppResult {
  auto* game = static_cast<luanaut::Game*>(appState);

  game->HandleEventSubTree(*event);

  return SDL_APP_CONTINUE;
}

auto SDL_AppQuit(void* appState, SDL_AppResult /*result*/) -> void {
  delete static_cast<luanaut::Game*>(appState);
}
