#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include <string>
#include <vector>
#include "Material.h"

namespace luanaut {

class ResourceManager {
 public:
  // todo store mesh

  // todo pass some args in so we can create different materials
  auto CreateMaterial(SDL_Window* window, SDL_GPUDevice* device)
      -> const Material*;

 private:
  static auto readFile(const std::string& fileName) -> std::vector<uint8_t>;

  // For keeping in RAII scope
  std::vector<std::unique_ptr<Material>> materials_;
};

}  // namespace luanaut
