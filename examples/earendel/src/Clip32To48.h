#pragma once
#include <lneng/AssetLoader.h>
#include <lneng/Camera.h>
#include <lneng/Easings.h>
#include <lneng/FullscreenNode.h>
#include <lneng/Game.h>
#include <lneng/LightNode.h>
#include <lneng/ModelNode.h>
#include <lneng/ParticleNode.h>
#include <lneng/Scene.h>
#include <lneng/SkyboxNode.h>
#include <lneng/Transform.h>
#include "Random.h"

namespace erdl {

using namespace std::chrono_literals;

// NOLINTBEGIN(readability-magic-numbers)
class Clip32To48 : public lneng::Scene {
 public:
  Clip32To48(std::chrono::milliseconds beatDuration)
      : Scene(lneng::Transform{.position = {0, 0, 0}}, 60),
        beatDuration_(beatDuration) {}

  auto LoadScene() -> void override {
    auto* assetLoader = deps_->Resolve<lneng::AssetLoader>();
    auto* camera = deps_->Resolve<lneng::Camera>();

    AddChild(std::make_unique<lneng::LightNode>(lneng::LightInfo{
        .direction = {0, -1, 0},
        .color = {1, 1, 1},
    }));
    // Don't blast full white. The belly is already white. Too bright
    // todo this is gonna be a problem when we have different models in the
    // scene sharing the same light
    AddChild(std::make_unique<lneng::LightNode>(lneng::LightInfo{
        .direction = {0, 1, 0},
        .color = {0.5F, 0.5F, 0.5F},
    }));

    // gonna do a move + rotate same time. tricky to do all in one node
    auto whaleMoveNode = std::make_unique<lneng::Node>();
    auto* whaleMovePtr = whaleMoveNode.get();
    AddChild(std::move(whaleMoveNode));

    std::filesystem::path whalePath =
        std::filesystem::path(APP_ASSETS_BIN_DIR) / "models" / "whale.glb";
    auto whale = assetLoader->LoadGlb(whalePath);
    auto whaleNode = std::make_unique<lneng::ModelNode>(whale);
    auto* whalePtr = whaleNode.get();
    whalePtr->GetMaterial().crescentMin = 0.6F;
    whalePtr->GetMaterial().edgeWidth = 0.025F;
    whaleMovePtr->AddChild(std::move(whaleNode));

    whaleMovePtr->GetTransform().position = {15, -10, 25};

    glm::quat target =
        glm::angleAxis(glm::radians(-90.0F), glm::vec3(1, 0, 0)) *
        glm::angleAxis(glm::radians(-90.0F), glm::vec3(0, 0, 1));
    whalePtr->GetTransform().rotation = target;

    whaleMovePtr->MoveTo({-10, -10, 25}, beatDuration_ * 16);
    whalePtr->RotateByWorld(4, glm::vec3(0, 0, 1))
        .Then()
        .RotateByWorld(-4, glm::vec3(0, 0, 1), beatDuration_ * 8,
                       lneng::easeInOutQuad)
        .Then()
        .RotateByWorld(4, glm::vec3(0, 0, 1), beatDuration_ * 8,
                       lneng::easeInOutQuad);

    camera->SetShake(true, {0.2F, 0.2F, 0.2F});
    ScheduleTask([camera]() { camera->GetTransform().position = {0, 0, -20}; },
                 beatDuration_ * 8);
    ScheduleTask([camera]() { camera->SetShake(false); }, beatDuration_ * 16);

    constexpr int numParticles = 300;
    for (int i = 0; i < numParticles; ++i) {
      auto particleNode = std::make_unique<lneng::ParticleNode>(
          glm::vec3{1, 1, 1}, 5, GetRandomFloat(0.2F, 1.0F));
      auto* ptr = particleNode.get();
      AddChild(std::move(particleNode));

      float progress = static_cast<float>(i) / static_cast<float>(numParticles);

      constexpr float spawnWidth = 1000;
      // why is positioning all this so hard lol
      float spawnX = -spawnWidth + (GetRandomFloat(0, 1) * spawnWidth);
      float targetX = spawnX + GetRandomFloat(1000, 2000);
      float totalDistanceX = targetX - spawnX;
      float prewarmedX = spawnX + (progress * totalDistanceX);

      constexpr float spawnY = -500;
      float targetY = 500;
      float totalDistanceY = targetY - spawnY;
      float prewarmedY = spawnY + (progress * totalDistanceY);

      float spawnZ = GetRandomFloat(100, 300);
      ptr->GetTransform().position = {prewarmedX, prewarmedY, spawnZ};

      constexpr int targetDurationMs = 30000;
      float remainingPercentage = (targetY - prewarmedY) / totalDistanceY;
      auto duration = std::chrono::milliseconds(
          static_cast<int>(targetDurationMs * remainingPercentage));

      ptr->MoveTo({targetX, targetY, spawnZ}, duration).Then().Call([ptr]() {
        ptr->Destroy();
      });
    }
  }

 private:
  std::chrono::milliseconds beatDuration_;
};
// NOLINTEND(readability-magic-numbers)

}  // namespace erdl
