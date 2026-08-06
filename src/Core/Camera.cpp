// Smoothly follows a target while keeping the view inside level bounds.
#include "Core/Camera.h"

#include "Utils/MathUtils.h"

#include <algorithm>
#include <cmath>

sf::Vector2f Camera::follow(sf::Vector2f current, sf::Vector2f target, sf::Vector2f viewSize, sf::Vector2f worldSize, float dt) {
    const sf::Vector2f clamped(
        MathUtils::clamp(target.x - viewSize.x * 0.5f, 0.0f, std::max(0.0f, worldSize.x - viewSize.x)),
        MathUtils::clamp(target.y - viewSize.y * 0.5f + 40.0f, 0.0f, std::max(0.0f, worldSize.y - viewSize.y))
    );
    const float ease = 1.0f - std::pow(0.001f, dt);
    return current + (clamped - current) * ease;
}

void Camera::reset() {}
