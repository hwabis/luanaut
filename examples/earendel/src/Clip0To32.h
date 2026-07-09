#pragma once
#include <lneng/AssetLoader.h>
#include <lneng/Camera.h>
#include <lneng/Game.h>
#include <lneng/LightNode.h>
#include <lneng/ModelNode.h>
#include <lneng/Scene.h>
#include <lneng/SkyboxNode.h>
#include <lneng/Transform.h>

namespace erdl {

using namespace std::chrono_literals;

// NOLINTBEGIN(readability-magic-numbers)
class Clip0To32 : public lneng::Scene {
 public:
  Clip0To32() : Scene(lneng::Transform{.position = {0, 0, 0}}, 60) {}

  auto LoadScene() -> void override {
    auto* assetLoader = deps_->Resolve<lneng::AssetLoader>();
    // auto* camera = deps_->Resolve<lneng::Camera>();

    AddChild(std::make_unique<lneng::LightNode>(lneng::LightInfo{
        .direction = {0, -1, -0.5F},
        .color = {1, 1, 1},
    }));

    std::filesystem::path earthPath =
        std::filesystem::path(APP_ASSETS_BIN_DIR) / "models" / "earth.glb";
    auto earth = assetLoader->LoadGlb(earthPath);
    auto earthNode = std::make_unique<lneng::ModelNode>(earth);
    auto* earthPtr = earthNode.get();
    earthNode->GetMaterial().crescentMin = 0.25F;
    AddChild(std::move(earthNode));

    earthPtr->GetTransform().scale = {100, 100.0, 100};
    earthPtr->GetTransform().position = {0, -200, 500};
  }
};
// NOLINTEND(readability-magic-numbers)

}  // namespace erdl
