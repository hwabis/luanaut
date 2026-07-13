#pragma once

#include <lneng/Scene.h>
#include <lneng/SkyboxNode.h>
#include <lneng/Transform.h>
#include <chrono>
#include "ClipStartTo32.h"

namespace erdl {

using namespace std::chrono_literals;

// NOLINTBEGIN(readability-magic-numbers)
class EarendelScene : public lneng::Scene {
 public:
  EarendelScene() : Scene(lneng::Transform{.position = {0, -200, -500}}, 60) {}

  auto LoadScene() -> void override {
    auto* assetLoader = deps_->Resolve<lneng::AssetLoader>();

    std::vector<std::filesystem::path> skyboxPaths = {
        std::filesystem::path(APP_ASSETS_BIN_DIR) / "models" / "skyRT.tga",
        std::filesystem::path(APP_ASSETS_BIN_DIR) / "models" / "skyLF.tga",
        std::filesystem::path(APP_ASSETS_BIN_DIR) / "models" / "skyUP.tga",
        std::filesystem::path(APP_ASSETS_BIN_DIR) / "models" / "skyDN.tga",
        std::filesystem::path(APP_ASSETS_BIN_DIR) / "models" / "skyFT.tga",
        std::filesystem::path(APP_ASSETS_BIN_DIR) / "models" / "skyBK.tga",
    };
    auto skybox =
        assetLoader->LoadSkybox(skyboxPaths[0], skyboxPaths[1], skyboxPaths[2],
                                skyboxPaths[3], skyboxPaths[4], skyboxPaths[5]);
    auto skyboxNode = std::make_unique<lneng::SkyboxNode>(skybox);
    AddChild(std::move(skyboxNode));

    auto currentSceneNode = std::make_unique<lneng::Node>();
    auto* currentSceneNodePtr = currentSceneNode.get();
    AddChild(std::move(currentSceneNode));

    constexpr float trackBpm = 180;
    constexpr auto beatDuration =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float, std::milli>(60000.0F / trackBpm));
    constexpr std::chrono::milliseconds trackStartOffset =
        (3 * beatDuration) + 160ms;

    ScheduleTask(
        [currentSceneNodePtr, trackStartOffset, beatDuration]() {
          currentSceneNodePtr->AddChild(
              std::make_unique<ClipStartTo32>(trackStartOffset, beatDuration));
        },
        0ms);
    // todo
    // ScheduleTask(
    //     [currentSceneNodePtr, beatDuration]() {
    //       currentSceneNodePtr->ClearChildren();
    //       currentSceneNodePtr->AddChild(
    //           std::make_unique<Clip0To32>(beatDuration));
    //     },
    //     trackStartOffset + beatDuration * 32);

    auto* audioLoader = deps_->Resolve<lneng::AudioLoader>();
    auto* track =
        audioLoader->LoadWav(std::filesystem::path(APP_ASSETS_BIN_DIR) /
                             "audio" / "Earendel - Palami feat.neur6sia.wav");
    track->Play();
  }
};
// NOLINTEND(readability-magic-numbers)

}  // namespace erdl
