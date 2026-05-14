#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <spdlog/spdlog.h>
#include "Game.h"
#include "Node.h"

// todo all these SDL_ init methods needs to be abstracted out of the app lol
// maybe some weird macro thing

auto SDL_AppInit(void** appState, int /*argc*/, char** /*argv*/)
    -> SDL_AppResult {
  if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO)) {
    spdlog::error(SDL_GetError());
    return SDL_APP_FAILURE;
  }

  constexpr int width = 1280;
  constexpr int height = 720;
  auto* window =
      SDL_CreateWindow("Luanaut", width, height, SDL_WINDOW_RESIZABLE);
  if (window == nullptr) {
    spdlog::error(SDL_GetError());
    return SDL_APP_FAILURE;
  }
  // todo store window ptr somewhere (probably app later)

  auto* game = new luanaut::Game();
  // todo user adds all the nodes that game would need...
  game->AddChild(std::make_unique<luanaut::Node>());

  *appState = game;

  return SDL_APP_CONTINUE;
}

auto SDL_AppIterate(void* appState) -> SDL_AppResult {
  auto* game = static_cast<luanaut::Game*>(appState);

  if (!game->IsRunning()) {
    return SDL_APP_SUCCESS;
  }

  game->UpdateSubTree();

  return SDL_APP_CONTINUE;
}

auto SDL_AppEvent(void* /*appState*/, SDL_Event* event) -> SDL_AppResult {
  // todo pass event to game. return app_success determined by game.
  // but for now, hardcode this so I don't have to task manager every time
  if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
    return SDL_APP_SUCCESS;
  }

  return SDL_APP_CONTINUE;
}

auto SDL_AppQuit(void* appState, SDL_AppResult /*result*/) -> void {
  delete static_cast<luanaut::Game*>(appState);
}
