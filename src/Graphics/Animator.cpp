// Advances frame timing for the currently selected animation.
#include "Graphics/Animator.h"

void Animator::play(const Animation& animation) {
    if (isPlaying(animation)) return;
    current = animation;
    timer = 0.0f;
    frame = 0;
}

void Animator::update(float dt) {
    if (current.frameCount <= 1) return;
    timer += dt;
    if (timer >= current.frameTime) {
        timer -= current.frameTime;
        frame = (frame + 1) % current.frameCount;
    }
}

sf::IntRect Animator::getFrameRect() const {
    return current.frameRect(frame);
}

bool Animator::isPlaying(const Animation& animation) const {
    return current.firstFrame.position == animation.firstFrame.position &&
           current.firstFrame.size == animation.firstFrame.size &&
           current.frameCount == animation.frameCount &&
           current.frameTime == animation.frameTime;
}
