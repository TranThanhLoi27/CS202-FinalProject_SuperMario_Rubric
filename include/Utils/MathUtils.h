// Small math helpers used by movement, collision, and camera code.
#pragma once

#include <SFML/Graphics/Rect.hpp>

namespace MathUtils {
/// Clamps a comparable value to an inclusive range.
template <typename T>
constexpr T clamp(T value, T low, T high) {
    return value < low ? low : (value > high ? high : value);
}

/// Reports whether two floating-point axis-aligned rectangles overlap.
bool intersects(const sf::FloatRect& a, const sf::FloatRect& b);
}
