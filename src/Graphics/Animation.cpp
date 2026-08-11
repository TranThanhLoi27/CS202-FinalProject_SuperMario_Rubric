// Animation currently stores metadata only; Animator owns frame advancement.
#include "Graphics/Animation.h"

Animation::Animation(sf::IntRect firstFrame, int frameCount, float frameTime)
    : firstFrame(firstFrame), frameCount(frameCount), frameTime(frameTime) {}
