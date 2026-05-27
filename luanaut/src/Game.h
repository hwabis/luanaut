#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <memory>
#include "GpuResourceManager.h"
#include "Node.h"
#include "Renderer.h"
#include "Scene.h"
#include "SceneManager.h"

namespace luanaut {

class Game : public Node {
 public:
  Game(std::unique_ptr<Scene> initialScene);
  ~Game() override;

 protected:
  auto Update() -> void override;
  auto HandleEvent(const SDL_Event& event) -> bool override;

 private:
  std::unique_ptr<Renderer> renderer_;
  std::unique_ptr<GpuResourceManager> gpuResourceManager_;
  SceneManager* sceneManager_;
};

}  // namespace luanaut
