#pragma once
#include <random>

inline auto GetRandomInt(int min, int max) -> int {
  static std::random_device random;
  static std::mt19937 gen(random());
  std::uniform_int_distribution<int> distr(min, max);
  return distr(gen);
}

inline auto GetRandomFloat(float min, float max) -> float {
  static std::random_device random;
  static std::mt19937 gen(random());
  std::uniform_real_distribution<float> distr(min, max);
  return distr(gen);
}
