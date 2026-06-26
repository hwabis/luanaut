#pragma once
#include <optional>
#include "CameraInfo.h"
#include "DrawInfo.h"
#include "LightInfo.h"
#include "SkyboxInfo.h"

namespace lneng {

struct SceneInfo {
  std::vector<DrawInfo> draws;
  std::vector<LightInfo> lights;
  CameraInfo camera;
  std::optional<SkyboxInfo> skybox;
};

}  // namespace lneng
