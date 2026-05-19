#pragma once
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace luanaut {

class DependencyContainer {
 public:
  DependencyContainer* parent = nullptr;

  template <typename T>
  auto Cache(std::shared_ptr<T> dep) -> void {
    deps_[typeid(T)] = std::move(dep);
  }

  template <typename T>
  auto Resolve() const -> std::shared_ptr<T> {
    auto itr = deps_.find(typeid(T));

    if (itr != deps_.end()) {
      return std::static_pointer_cast<T>(itr->second);
    }

    if (parent) {
      return parent->Resolve<T>();
    }

    return nullptr;
  }

 private:
  std::unordered_map<std::type_index, std::shared_ptr<void>> deps_;
};

}  // namespace luanaut
