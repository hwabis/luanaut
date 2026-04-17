#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>

#include "VulkanStuff.h"

auto main() -> int {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
    spdlog::error("SDL_Init failed: {}", SDL_GetError());
    return 1;
  }

  constexpr int width = 1280;
  constexpr int height = 720;
  SDL_Window* window =
      SDL_CreateWindow("Luanaut Engine", width, height,
                       SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
  if (window == nullptr) {
    spdlog::error("Window creation failed: {}", SDL_GetError());
    return 1;
  }

  {
    luanaut::VulkanStuff vulkan(window);
    bool running = true;
    while (running) {
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
          running = false;
        }
        if (event.type == SDL_EVENT_WINDOW_RESIZED) {
          vulkan.NotifyResize();
        }
      }
      vulkan.DrawFrame();

      constexpr int sleepTime = 20;
      std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
