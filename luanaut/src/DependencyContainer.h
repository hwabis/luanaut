#pragma once
#include <typeindex>
#include <unordered_map>

namespace luanaut {

class DependencyContainer {
 public:
  DependencyContainer* parent = nullptr;

  template <typename T>
  auto Cache(T* dep) -> void {
    deps_[typeid(T)] = dep;
  }

  template <typename T>
  auto Resolve() const -> T* {
    auto itr = deps_.find(typeid(T));

    if (itr != deps_.end()) {
      return static_cast<T*>(itr->second);
    }

    if (parent) {
      return parent->Resolve<T>();
    }

    return nullptr;
  }

 private:
  std::unordered_map<std::type_index, void*> deps_;
};

}  // namespace luanaut
