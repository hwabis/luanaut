#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <spdlog/spdlog.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        spdlog::error("SDL_Init failed: {}", SDL_GetError());
        return 1;
    }

    // Create a window with Vulkan support
    SDL_Window* window = SDL_CreateWindow(
        "Luanaut Engine", 
        1280, 720, 
        SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        spdlog::error("Window creation failed: {}", SDL_GetError());
        return 1;
    }

    spdlog::info("SDL3 + Vulkan Window Initialized!");
    std::cout << "???\n";

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
        // Vulkan Draw Loop will go here
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
