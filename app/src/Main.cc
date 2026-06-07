#include <lneng/AssetLoader.h>
#include <lneng/Camera.h>
#include <lneng/Game.h>
#include <lneng/LightNode.h>
#include <lneng/ModelNode.h>

class MyAwesomeGame : public lneng::Game {
 public:
  MyAwesomeGame() : lneng::Game(std::make_unique<MyAwesomeScene>()) {}

 private:
  class MyAwesomeScene : public lneng::Scene {
   public:
    auto Load() -> void override {
      auto* assetLoader = deps_->Resolve<lneng::AssetLoader>();
      std::filesystem::path duckPath =
          std::filesystem::path(APP_ASSETS_BIN_DIR) / "models" / "Duck.glb";
      auto duck = assetLoader->LoadGlb(duckPath);
      auto duckNode = std::make_unique<lneng::ModelNode>(duck);
      auto* duckNodePtr = duckNode.get();
      AddChild(std::move(duckNode));

      using namespace std::chrono_literals;
      // Annoying sequencing right now but it works!!
      duckNodePtr->Rotate(
          clock_->now, clock_->now + 3s, duckNodePtr->transform.rotation,
          duckNodePtr->transform.rotation *
              glm::angleAxis(glm::radians(179.9F), glm::vec3(0, 1, 0)));
      duckNodePtr->Rotate(
          clock_->now + 3s, clock_->now + 4s,
          duckNodePtr->transform.rotation *
              glm::angleAxis(glm::radians(180.1F), glm::vec3(0, 1, 0)),
          duckNodePtr->transform.rotation);

      AddChild(std::make_unique<lneng::LightNode>(lneng::LightInfo{
          .direction = {0, 0, 1},
          .color = {1, 1, 1},
      }));

      auto cameraNode = std::make_unique<lneng::Camera>(60);
      auto* cameraNodePtr = cameraNode.get();
      AddChild(std::move(cameraNode));
      cameraNodePtr->transform.position = glm::vec3(0, 100, -300);
    }
  };
};

#define LNENG_GAME_CLASS MyAwesomeGame
#include <lneng/EntryPoint.h>
