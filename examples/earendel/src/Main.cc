#include <lneng/EntryPoint.h>
#include "EarendelScene.h"

class EarendelGame : public lneng::Game {
 public:
  EarendelGame() : lneng::Game(std::make_unique<erdl::EarendelScene>()) {}
};

LNENG_DEFINE_ENTRYPOINT(EarendelGame)
