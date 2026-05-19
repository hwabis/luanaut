#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include <string>
#include <unordered_map>
#include "Material.h"

namespace luanaut {

class GpuResourceManager {
 public:
  GpuResourceManager(SDL_Window* window, SDL_GPUDevice* device);

  // todo store mesh

  struct MaterialInfo {
    std::string vertShaderPath;
    std::string fragShaderPath;
    // todo texture paths etc
    [[nodiscard]] auto getKey() const -> std::string {
      return vertShaderPath + fragShaderPath;
    }
  };

  auto CreateMaterial(const MaterialInfo& info) -> const Material*;

 private:
  static auto readFile(const std::string& fileName) -> std::vector<uint8_t>;

  std::unordered_map<std::string, std::unique_ptr<Material>> cache_;

  SDL_Window* window_;
  SDL_GPUDevice* device_;
};

}  // namespace luanaut
