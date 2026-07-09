#include "EarendelScene.h"

class EarendelGame : public lneng::Game {
 public:
  EarendelGame() : lneng::Game(std::make_unique<erdl::EarendelScene>()) {}
};

#define LNENG_GAME_CLASS EarendelGame
#include <lneng/EntryPoint.h>
