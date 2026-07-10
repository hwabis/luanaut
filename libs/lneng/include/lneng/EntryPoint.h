#pragma once
#ifndef LNENG_GAME_CLASS
// todo HOW DO I PROPERLY GET RID OF THIS RED SQUIGGLE IN IDEs
#error "Define LNENG_GAME_CLASS before including EntryPoint.h"
#endif

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <lneng/Game.h>

auto SDL_AppInit(void** appState, int /*argc*/, char** /*argv*/)
    -> SDL_AppResult {
  try {
    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO)) {
      return SDL_APP_FAILURE;
    }
    *appState = new LNENG_GAME_CLASS();
    return SDL_APP_CONTINUE;
  } catch (const std::exception& e) {
    SDL_Log("FATAL (init): %s", e.what());
    return SDL_APP_FAILURE;
  } catch (...) {
    SDL_Log("FATAL (init): unknown exception");
    return SDL_APP_FAILURE;
  }
}

auto SDL_AppIterate(void* appState) -> SDL_AppResult {
  try {
    auto* game = static_cast<lneng::Game*>(appState);
    if (!game->IsAlive()) {
      return SDL_APP_SUCCESS;
    }
    game->UpdateSubTree();
    return SDL_APP_CONTINUE;
  } catch (const std::exception& e) {
    SDL_Log("FATAL (init): %s", e.what());
    return SDL_APP_FAILURE;
  } catch (...) {
    SDL_Log("FATAL (init): unknown exception");
    return SDL_APP_FAILURE;
  }
}

auto SDL_AppEvent(void* appState, SDL_Event* event) -> SDL_AppResult {
  try {
    auto* game = static_cast<lneng::Game*>(appState);
    game->HandleEventSubTree(*event);
    return SDL_APP_CONTINUE;
  } catch (const std::exception& e) {
    SDL_Log("FATAL (init): %s", e.what());
    return SDL_APP_FAILURE;
  } catch (...) {
    SDL_Log("FATAL (init): unknown exception");
    return SDL_APP_FAILURE;
  }
}

auto SDL_AppQuit(void* appState, SDL_AppResult /*result*/) -> void {
  delete static_cast<lneng::Game*>(appState);
}
