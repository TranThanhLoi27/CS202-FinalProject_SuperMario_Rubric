// Advances animation frame timing for future textured sprites.
#pragma once

#include "Graphics/Animation.h"

class Animator {
public:
    void play(const Animation& animation);
    void update(float dt);

private:
    Animation current;
    float timer = 0.0f;
    int frame = 0;
};
