#include "lneng/Scene.h"
#include "lneng/Camera.h"

namespace lneng {

Scene::Scene(Transform cameraStart, float cameraFov)
    : cameraStart_(cameraStart), cameraFov_(cameraFov) {}

auto Scene::Load() -> void {
  auto cameraNode = std::make_unique<lneng::Camera>(cameraFov_);
  auto* cameraNodePtr = cameraNode.get();
  AddChild(std::move(cameraNode));
  cameraNodePtr->GetTransform() = cameraStart_;
  deps_->Cache(cameraNodePtr);

  LoadScene();
}

auto Scene::LoadScene() -> void {};

}  // namespace lneng
