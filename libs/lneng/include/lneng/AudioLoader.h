#pragma once
#include <filesystem>
#include <unordered_map>
#include "Track.h"

namespace lneng {

// todo make this
class AudioLoader {
 public:
  auto LoadWav(const std::filesystem::path& path) -> Track*;

 private:
  // todo audio here forever
  std::unordered_map<std::string, std::unique_ptr<Track>> tracks_;
};

}  // namespace lneng
