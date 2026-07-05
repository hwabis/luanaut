#include "MainScene.h"

class ErdlGame : public lneng::Game {
 public:
  ErdlGame() : lneng::Game(std::make_unique<erdl::MainScene>()) {}
};

#define LNENG_GAME_CLASS ErdlGame
#include <lneng/EntryPoint.h>
