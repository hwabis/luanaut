#pragma once
#include "CameraInfo.h"
#include "DrawInfo.h"
#include "LightInfo.h"

namespace lneng {

struct SceneInfo {
  std::vector<DrawInfo> draws;
  std::vector<LightInfo> lights;
  CameraInfo camera;
};

}  // namespace lneng
