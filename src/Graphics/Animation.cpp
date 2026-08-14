// Animation currently stores metadata only; Animator owns frame advancement.
#include "Graphics/Animation.h"

Animation::Animation(sf::IntRect firstFrame, int frameCount, float frameTime)
    : firstFrame(firstFrame), frameCount(frameCount), frameTime(frameTime) {}

sf::IntRect Animation::frameRect(int frameIndex) const {
    if (frameCount <= 0) return firstFrame;
    const int clamped = frameIndex % frameCount;
    return sf::IntRect(
        {firstFrame.position.x + clamped * firstFrame.size.x, firstFrame.position.y},
        firstFrame.size
    );
}
