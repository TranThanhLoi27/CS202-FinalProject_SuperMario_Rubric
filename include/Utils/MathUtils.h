// Small math helpers used by movement, collision, and camera code.
#pragma once

#include <SFML/Graphics/Rect.hpp>

namespace MathUtils {
template <typename T>
constexpr T clamp(T value, T low, T high) {
    return value < low ? low : (value > high ? high : value);
}

bool intersects(const sf::FloatRect& a, const sf::FloatRect& b);
}
