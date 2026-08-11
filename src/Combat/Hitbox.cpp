// Hitbox currently stores bounds and lifetime only; DamageSystem owns collision checks.
#include "Combat/Hitbox.h"

Hitbox::Hitbox(sf::FloatRect bounds, float duration) : bounds(bounds), duration(duration) {}
