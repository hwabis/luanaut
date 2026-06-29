#pragma once
#include <lneng/AssetLoader.h>
#include <lneng/Camera.h>
#include <lneng/Game.h>
#include <lneng/LightNode.h>
#include <lneng/ModelNode.h>
#include <lneng/Scene.h>
#include <lneng/SkyboxNode.h>
#include <lneng/Transform.h>

using namespace std::chrono_literals;

// NOLINTBEGIN(readability-magic-numbers)
class MyAwesomeScene : public lneng::Scene {
 public:
  MyAwesomeScene() : Scene(lneng::Transform{.position = {0, 100, -300}}, 60) {}

  auto LoadScene() -> void override {
    auto* assetLoader = deps_->Resolve<lneng::AssetLoader>();

    std::filesystem::path duckPath =
        std::filesystem::path(APP_ASSETS_BIN_DIR) / "models" / "Duck.glb";
    auto duck = assetLoader->LoadGlb(duckPath);
    auto duckNode = std::make_unique<lneng::ModelNode>(duck);
    auto* duckNodePtr = duckNode.get();
    AddChild(std::move(duckNode));

    duckNodePtr->RotateTo(180.0F, glm::vec3(0, 1, 0), 2s, lneng::easeOutQuad)
        .ScaleTo({0.5F, 0.5F, 0.5F}, 3s)
        .Delay(1s)
        .RotateTo(180.0F, glm::vec3(1, 0, 0), 1s)
        .ScaleTo({1.0F, 1.0F, 1.0F}, 500ms);

    AddChild(std::make_unique<lneng::LightNode>(lneng::LightInfo{
        .direction = {0, 0, 1},
        .color = {1, 1, 1},
    }));

    auto* camera = deps_->Resolve<lneng::Camera>();
    camera->Delay(1s)
        .MoveTo({0, 100, -500}, 0s)
        .Then()
        .MoveTo({0, 100, -200}, 3s)
        .Then()
        .RotateTo(90.0F, {-1, 0, 0}, 2s, lneng::easeInQuad);

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
  }
};
// NOLINTEND(readability-magic-numbers)
