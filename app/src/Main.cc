#include <lneng/Game.h>
#include <lneng/MeshNode.h>

class MyAwesomeGame : public lneng::Game {
 public:
  MyAwesomeGame() : lneng::Game(std::make_unique<MyAwesomeScene>()) {}

 private:
  class MyAwesomeScene : public lneng::Scene {
   public:
    auto Load() -> void override {
      AddChild(std::make_unique<lneng::MeshNode>());
    }
  };
};

#define LNENG_GAME_CLASS MyAwesomeGame
#include <lneng/EntryPoint.h>
