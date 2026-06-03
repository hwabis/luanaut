#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <memory>
#include "AssetLoader.h"
#include "Clock.h"
#include "GpuResourceLoader.h"
#include "Node.h"
#include "Renderer.h"
#include "Scene.h"
#include "SceneManager.h"

namespace lneng {

class Game : public Node {
 public:
  Game(std::unique_ptr<Scene> initialScene);
  ~Game() override;

  using Node::DrawSubTree;
  using Node::HandleEventSubTree;
  using Node::UpdateSubTree;

 protected:
  auto Update() -> void override;
  auto HandleEvent(const SDL_Event& event) -> bool override;

 private:
  Renderer renderer_;
  AssetLoader assetLoader_;
  GpuResourceLoader gpuResourceLoader_;
  Clock ownedClock_;
  SceneManager* sceneManager_;
};

}  // namespace lneng
