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

  // todo we pass in our app root node which has children and everything
  auto root = std::make_unique<luanaut::Node>();
  *appState = new luanaut::Game(std::move(root));

  return SDL_APP_CONTINUE;
}

auto SDL_AppIterate(void* appState) -> SDL_AppResult {
  auto* app = static_cast<luanaut::Game*>(appState);
  app->Update();

  return SDL_APP_CONTINUE;
}

auto SDL_AppEvent(void* /*appState*/, SDL_Event* event) -> SDL_AppResult {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }

  return SDL_APP_CONTINUE;
}

auto SDL_AppQuit(void* appState, SDL_AppResult /*result*/) -> void {
  auto* game = static_cast<luanaut::Game*>(appState);
  delete game;
}
