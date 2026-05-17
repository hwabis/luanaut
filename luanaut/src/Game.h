#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <memory>
#include <string>
#include "Node.h"
#include "SdlHandles.h"

namespace luanaut {

class Game {
 public:
  Game(std::unique_ptr<Node> root);

  auto HandleEvent(const SDL_Event& event) -> void;

  auto Update() -> void;
  auto SetScene(std::unique_ptr<Node> root) -> void;
  [[nodiscard]] auto IsRunning() const -> bool;

 private:
  auto createPipeline() -> SDL_GPUGraphicsPipeline*;

  std::unique_ptr<Node> root_;

  SdlWindowHandle window_;
  SdlGpuDeviceHandle device_;
  std::unordered_map<std::string, SdlGpuGraphicsPipelineHandle> pipelines_;

  bool isRunning_ = true;
};

}  // namespace luanaut
