#pragma once
#include <SDL3/SDL_audio.h>
#include <chrono>
#include <filesystem>
#include <optional>

namespace lneng {

// todo this should be manually ticked with Update() by owning Node
// to always stay in sync with its clock (?)
class Track {
 public:
  Track(const std::filesystem::path& path);
  ~Track();

  Track(const Track&) = delete;
  auto operator=(const Track&) -> Track& = delete;

  auto Play() -> void;
  auto Pause() -> void;
  // todo
  // auto Stop() -> void;
  // auto Seek(std::chrono::milliseconds pos) -> void;
  // [[nodiscard]] auto GetPosition() const -> std::chrono::milliseconds;

 private:
  SDL_AudioSpec spec_;
  Uint8* buffer_;
  Uint32 length_;
  SDL_AudioStream* stream_;
};

}  // namespace lneng
