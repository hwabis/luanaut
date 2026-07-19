#pragma once
#include <optional>
#include "CameraInfo.h"
#include "DrawInfo.h"
#include "FullscreenInfo.h"
#include "LightInfo.h"
#include "ParticleInfo.h"
#include "SkyboxInfo.h"

namespace lneng {

struct SceneInfo {
  std::vector<DrawInfo> draws;
  std::vector<LightInfo> lights;
  CameraInfo camera;
  std::optional<SkyboxInfo> skybox;
  std::vector<FullscreenInfo> fullscreens;
  std::vector<ParticleInfo> particles;
};

}  // namespace lneng
