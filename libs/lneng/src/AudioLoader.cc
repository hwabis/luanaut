#include "lneng/AudioLoader.h"

namespace lneng {

auto AudioLoader::LoadWav(const std::filesystem::path& path) -> Track* {
  std::string pathStr = path.string();

  if (auto itr = tracks_.find(pathStr); itr != tracks_.end()) {
    return itr->second.get();
  }

  auto [itr, success] =
      tracks_.emplace(pathStr, std::make_unique<Track>(pathStr));
  return itr->second.get();
}

}  // namespace lneng
