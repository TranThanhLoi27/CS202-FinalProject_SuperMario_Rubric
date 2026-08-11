// Advances frame timing for the currently selected animation.
#include "Graphics/Animator.h"

void Animator::play(const Animation& animation) {
    current = animation;
    timer = 0.0f;
    frame = 0;
}

void Animator::update(float dt) {
    timer += dt;
    if (timer >= current.frameTime && current.frameCount > 0) {
        timer = 0.0f;
        frame = (frame + 1) % current.frameCount;
    }
}
