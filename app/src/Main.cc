#include <lneng/AssetLoader.h>
#include <lneng/Game.h>
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

      AddChild(std::make_unique<RotatingModelNode>(duck));
    }
  };

  class RotatingModelNode : public lneng::ModelNode {
   public:
    RotatingModelNode(lneng::ModelCreateInfo modelInfo)
        : lneng::ModelNode(std::move(modelInfo)) {}

    auto Update() -> void override {
      auto* clock = deps_->Resolve<lneng::Clock>();

      constexpr float speed = 0.1F;
      transform.rotation =
          glm::angleAxis(glm::radians(speed * static_cast<float>(
                                                  clock->deltaTime.count())),
                         glm::vec3(0, 1, 0)) *
          transform.rotation;

      // todo eventually we want the transform/keyframe system, something like:
      // box.RotateTo(90, duration: 1000, easing: Easing.OutQuint)
      //    .Loop(0); // 0ms pause between iterations
    }
  };
};

#define LNENG_GAME_CLASS MyAwesomeGame
#include <lneng/EntryPoint.h>
