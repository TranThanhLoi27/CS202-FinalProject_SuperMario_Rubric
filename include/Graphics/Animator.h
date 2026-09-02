// Advances animation frame timing for future textured sprites.
#pragma once

#include "Graphics/Animation.h"

class Animator {
public:
    /// Starts an animation and resets timing only when the animation changes.
    void play(const Animation& animation);
    /// Advances the current animation by elapsed time.
    void update(float dt);
    /// Returns the current zero-based frame index.
    int getFrame() const { return frame; }
    /// Returns the current animation metadata.
    const Animation& getAnimation() const { return current; }
    /// Returns the texture rectangle for the current frame.
    sf::IntRect getFrameRect() const;
    /// Reports whether the supplied animation is currently selected.
    bool isPlaying(const Animation& animation) const;

private:
    Animation current;
    float timer = 0.0f;
    int frame = 0;
};
