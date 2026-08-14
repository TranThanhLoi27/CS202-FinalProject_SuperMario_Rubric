// Advances animation frame timing for future textured sprites.
#pragma once

#include "Graphics/Animation.h"

class Animator {
public:
    void play(const Animation& animation);
    void update(float dt);
    int getFrame() const { return frame; }
    const Animation& getAnimation() const { return current; }
    sf::IntRect getFrameRect() const;
    bool isPlaying(const Animation& animation) const;

private:
    Animation current;
    float timer = 0.0f;
    int frame = 0;
};
