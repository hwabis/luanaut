#include "MyAwesomeScene.h"

class MyAwesomeGame : public lneng::Game {
 public:
  MyAwesomeGame() : lneng::Game(std::make_unique<MyAwesomeScene>()) {}
};

#define LNENG_GAME_CLASS MyAwesomeGame
#include <lneng/EntryPoint.h>
