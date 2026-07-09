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
  Clip0To32(std::chrono::milliseconds beatDuration)
      : Scene(lneng::Transform{.position = {0, 0, 0}}, 60),
        beatDuration_(beatDuration) {}

  auto LoadScene() -> void override {
    // todo for this scene (actually every scene basically):
    // bloom, floating billboards

    auto* assetLoader = deps_->Resolve<lneng::AssetLoader>();
    auto* camera = deps_->Resolve<lneng::Camera>();

    AddChild(std::make_unique<lneng::LightNode>(lneng::LightInfo{
        .direction = {0, -1, -1},
        .color = {6, 6, 6},  // why do i have to turn the color up so much T_T
    }));

    std::filesystem::path earthPath =
        std::filesystem::path(APP_ASSETS_BIN_DIR) / "models" / "earth.glb";
    auto earth = assetLoader->LoadGlb(earthPath);
    auto earthNode = std::make_unique<lneng::ModelNode>(earth);
    auto* earthPtr = earthNode.get();
    AddChild(std::move(earthNode));

    earthPtr->GetTransform().scale = {300, 300.0, 300};
    earthPtr->GetTransform().position = {0, -300, 500};
    earthPtr->RotateBy(20, {0, -1, 0}, beatDuration_ * 32);

    ScheduleTask([camera]() { camera->GetTransform().position = {0, 0, -500}; },
                 beatDuration_ * 16);
  }

 private:
  std::chrono::milliseconds beatDuration_;
};
// NOLINTEND(readability-magic-numbers)

}  // namespace erdl
