#include <lneng/AssetLoader.h>
#include <lneng/Camera.h>
#include <lneng/Game.h>
#include <lneng/LightNode.h>
#include <lneng/ModelNode.h>
#include <lneng/SkyboxNode.h>
#include <lneng/Transform.h>

class MyAwesomeGame : public lneng::Game {
 public:
  MyAwesomeGame() : lneng::Game(std::make_unique<MyAwesomeScene>()) {}

 private:
  class MyAwesomeScene : public lneng::Scene {
   public:
    MyAwesomeScene()
        : Scene(lneng::Transform{.position = {0, 100, -300}}, 60) {}

    auto LoadScene() -> void override {
      auto* assetLoader = deps_->Resolve<lneng::AssetLoader>();

      std::filesystem::path duckPath =
          std::filesystem::path(APP_ASSETS_BIN_DIR) / "models" / "Duck.glb";
      auto duck = assetLoader->LoadGlb(duckPath);
      auto duckNode = std::make_unique<lneng::ModelNode>(duck);
      // auto* duckNodePtr = duckNode.get();
      AddChild(std::move(duckNode));

      using namespace std::chrono_literals;
      // duckNodePtr->Rotate(
      //     clock_->now, clock_->now + 3s,
      //     duckNodePtr->GetTransform().rotation,
      //     duckNodePtr->GetTransform().rotation *
      //         glm::angleAxis(glm::radians(179.9F), glm::vec3(0, 1, 0)));
      // duckNodePtr->Rotate(
      //     clock_->now + 3s, clock_->now + 4s,
      //     duckNodePtr->GetTransform().rotation *
      //         glm::angleAxis(glm::radians(180.1F), glm::vec3(0, 1, 0)),
      //     duckNodePtr->GetTransform().rotation);

      AddChild(std::make_unique<lneng::LightNode>(lneng::LightInfo{
          .direction = {0, 0, 1},
          .color = {1, 1, 1},
      }));

      // auto* camera = deps_->Resolve<lneng::Camera>();
      // camera->Move(clock_->now, clock_->now + 4s,
      // camera->GetTransform().position,
      //              camera->GetTransform().position + glm::vec3{0, 0, 300});

      std::vector<std::filesystem::path> skyboxPaths = {
          std::filesystem::path(APP_ASSETS_BIN_DIR) / "models" / "sky_xpos.png",
          std::filesystem::path(APP_ASSETS_BIN_DIR) / "models" / "sky_xneg.png",
          std::filesystem::path(APP_ASSETS_BIN_DIR) / "models" / "sky_ypos.png",
          std::filesystem::path(APP_ASSETS_BIN_DIR) / "models" / "sky_yneg.png",
          std::filesystem::path(APP_ASSETS_BIN_DIR) / "models" / "sky_zpos.png",
          std::filesystem::path(APP_ASSETS_BIN_DIR) / "models" / "sky_zneg.png",
      };
      auto skybox = assetLoader->LoadSkybox(skyboxPaths[0], skyboxPaths[1],
                                            skyboxPaths[2], skyboxPaths[3],
                                            skyboxPaths[4], skyboxPaths[5]);
      auto skyboxNode = std::make_unique<lneng::SkyboxNode>(skybox);
      AddChild(std::move(skyboxNode));
    }
  };
};

#define LNENG_GAME_CLASS MyAwesomeGame
#include <lneng/EntryPoint.h>
