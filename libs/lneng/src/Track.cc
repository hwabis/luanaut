#include "lneng/Track.h"
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_log.h>

namespace lneng {

Track::Track(const std::filesystem::path& path) {
  if (!SDL_LoadWAV(path.string().c_str(), &spec_, &buffer_, &length_)) {
    throw std::runtime_error(SDL_GetError());
  }

  stream_ = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec_,
                                      nullptr, nullptr);
  if (stream_ == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }

  if (!SDL_PutAudioStreamData(stream_, buffer_, static_cast<int>(length_))) {
    throw std::runtime_error(SDL_GetError());
  }

  constexpr float doNotBlastMyEars = 0.5F;
  SDL_SetAudioStreamGain(stream_, doNotBlastMyEars);
}

Track::~Track() {
  if (stream_ != nullptr) {
    SDL_DestroyAudioStream(stream_);
  }
  if (buffer_ != nullptr) {
    SDL_free(buffer_);
  }
}

auto Track::Play() -> void {
  SDL_ResumeAudioStreamDevice(stream_);
}

auto Track::Pause() -> void {
  SDL_PauseAudioStreamDevice(stream_);
}

}  // namespace lneng
