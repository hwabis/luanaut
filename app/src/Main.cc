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

      AddChild(std::make_unique<lneng::ModelNode>(duck));
    }
  };
};

#define LNENG_GAME_CLASS MyAwesomeGame
#include <lneng/EntryPoint.h>
