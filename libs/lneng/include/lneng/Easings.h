#pragma once

namespace lneng {

// NOLINTBEGIN(readability-magic-numbers)

constexpr auto easeLinear(double time) -> double {
  return time;
}

constexpr auto easeInQuad(double time) -> double {
  return time * time;
}

constexpr auto easeOutQuad(double time) -> double {
  return time * (2.0 - time);
}

// NOLINTEND(readability-magic-numbers)

}  // namespace lneng
